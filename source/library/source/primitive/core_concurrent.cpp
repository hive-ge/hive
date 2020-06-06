/**
 * Primitives and defines for use in debug mode.
 */

#include "core_concurrent.hpp"
#include <hive.h>

namespace hive
{

    alignas(HIVE_CACHE_LINE_SIZE) thread_local std::atomic<int> * JobDispatch::global_lock;
    thread_local JobDispatch::Job * JobDispatch::active_job = nullptr;

    alignas(HIVE_CACHE_LINE_SIZE) thread_local unsigned ThreadRunner::thread_count = 0;
    thread_local int ThreadRunner::id                                              = 0;

    alignas(HIVE_CACHE_LINE_SIZE) thread_local JobDispatch::Job JobDispatch::fall_back_job = {
        nullptr, nullptr, 0, 0, nullptr, JobDispatch::JOB_STATE::FALL_BACK_JOB, 0, 0, 0, nullptr};

    alignas(HIVE_CACHE_LINE_SIZE) unsigned JobDispatch::number_of_job_slots = 0;
    std::atomic_bool JobDispatch::cleanup_lock                              = 0;
    std::atomic<bool> ThreadRunner::KILL_SWITCHED                           = false;
    std::thread ** ThreadRunner::threads                                    = nullptr;
    alignas(HIVE_CACHE_LINE_SIZE) JobDispatch::Job * JobDispatch::job_pool  = nullptr;


    //#############################################################################//

    //#############################################################################//

    //#############################################################################//

    LinkedListConcurrentQueue<JobDispatch::Job, int> JobDispatch::free_queue;
    LinkedListConcurrentQueue<JobDispatch::Job, int> JobDispatch::priority_queues[10];

    //#############################################################################//

    //#############################################################################//

    //#############################################################################//

    void JobDispatch::enqueueJob(Job & job, JOB_PRIORITY priority)
    {
        if (job.state.load() == JOB_STATE::FALL_BACK_JOB) return runJob(job);

        job.state.store(JOB_STATE::PENDING);

        if (priority == JOB_PRIORITY::INTERNAL_PRIORITY)
            runJob(job);
        else {
            priority_queues[priority].enqueue(job, job);

            // If single threaded immediately execute this job.
            if (ThreadRunner::getNumberOfThreads() < 1) acquireJob();
        }
    }

    void JobDispatch::promoteJobToPending(Job & job, JOB_PRIORITY priority)
    {
        int i, decrement;

        do {

            i = job.local_counter.load(std::memory_order_relaxed);

            decrement = i - 1;

            if (i == 0) HIVE_ERROR("Failed to run job. Job state has become corrupted");

        } while (
            !(job.local_counter.compare_exchange_strong(i, decrement, std::memory_order_relaxed)));

        enqueueJob(job, priority);
    }


    JobDispatch::JOB & JobDispatch::acquireJobSlot()
    {
        thread_local unsigned exponential_sleep_count = 1;

        while (true) {

            JOB * job = &free_queue.dequeue();


            if (job->state.load() == JOB_STATE::DEAD_RINGER) {

                // Try to acquire the cleanup lock and clean up all free slots
                // GRABBED LOCK

                bool locked = false;

                if (cleanup_lock.compare_exchange_weak(locked, true)) {

                    JOB * first = nullptr;
                    JOB * last  = nullptr;

                    unsigned restored = 0;

                    for (int i = 0; i < number_of_job_slots; i++) {

                        JOB & job = job_pool[i];

                        if (job.state == JOB_STATE::COMPLETED) {

                            job.state.store(JOB_STATE::FREE_SLOT);

                            if (last) {
                                last->next.store(job.index);
                            } else {
                                first = &job;
                            }

                            last = &job;

                            restored++;
                        }
                    }

                    if (first && last) {

                        free_queue.enqueue(*first, *last);
                        std::cout << "Job queue has not starved " << restored << " "
                                  << ThreadRunner::getID() << std::endl;
                    }

                    cleanup_lock.store(false);
                }

                exponential_sleep_count = (exponential_sleep_count << 1) & 0xFFF;

                std::this_thread::sleep_for(std::chrono::microseconds(exponential_sleep_count));

                // return the fall back job;
                job = &fall_back_job;

            } else {
                job->state = JOB_STATE::SETUP;
            }
            job->data_size           = 0;
            job->entry               = nullptr;
            job->data                = nullptr;
            job->destination_counter = nullptr;
            job->job_group_lock      = nullptr;
            job->local_counter.store(SETUP_COUNTER_START, std::memory_order_release);

            return *job;
        }
    }

    void JobDispatch::runJob(Job & job)
    {

        JOB_STATE job_state = job.state.load();

        try {

            active_job = &job;

            global_lock = job.job_group_lock;


            if (job_state != JOB_STATE::FALL_BACK_JOB) {


                JOB_STATE A = JOB_STATE::PENDING;

                if (!job.state.compare_exchange_weak(A, JOB_STATE::ACTIVE,
                                                     std::memory_order_acquire,
                                                     std::memory_order::memory_order_relaxed)) {
                    HIVE_ERROR("Unexpected job acquisition conflict");
                }
            }

            job.local_counter.store(0, std::memory_order_relaxed);

            job.job_start = std::chrono::high_resolution_clock::now();

            job.entry(job.data, job.data_size);

            if (global_lock) {
                int locked = 1;
                // attempt to unlock the global lock if it has been acquired.
                global_lock->compare_exchange_strong(locked, 0);
            }

            if (active_job) {

                auto end = std::chrono::high_resolution_clock::now();

                if (job.destination_counter) {

                    int i, decrement;

                    do {

                        i = job.destination_counter->load(std::memory_order_relaxed);

                        decrement = i - 1;

                    } while (!(job.destination_counter->compare_exchange_strong(
                        i, decrement, std::memory_order_relaxed)));
                }

                if (job_state != JOB_STATE::FALL_BACK_JOB) {

                    JOB_STATE A = JOB_STATE::ACTIVE;

                    if (!job.state.compare_exchange_weak(A, JOB_STATE::COMPLETED,
                                                         std::memory_order_acquire,
                                                         std::memory_order::memory_order_relaxed)) {
                        HIVE_ERROR("Unexpected job completion conflict");
                    }
                }
            };

        } catch (int i) {
            HIVE_ERROR("Failed to run job");
        }

        global_lock = nullptr;

        active_job = nullptr;

        retireJobSlot(job);
    }


    bool JobDispatch::acquireJob()
    {
        int retries = 4;

        while (retries-- > 0) {

            for (int i = 0; i < 1; i++) {
                while (true) {

                    Job & job = priority_queues[i].dequeue();

                    if (job.state.load() != JOB_STATE::DEAD_RINGER) {

                        // Check to make sure the local counter is 1
                        if (job.local_counter.load() != 1) {
                            priority_queues[i].enqueue(job, job);
                            continue;
                        };

                        runJob(job);

                        return true;
                    }

                    break;
                }
            }
        }

        return false;
    }


    void JobDispatch::initialize(unsigned num_of_slots)
    {

        std::cout << "global_lock : " << (unsigned long long)&JobDispatch::global_lock << std::endl;
        std::cout << "active_job : " << (unsigned long long)&JobDispatch::active_job << std::endl;
        std::cout << "cleanup_lock : " << (unsigned long long)&JobDispatch::cleanup_lock
                  << std::endl;
        std::cout << "global_lock : " << (unsigned long long)&JobDispatch::global_lock << std::endl;

        if (!job_pool) {

            job_pool = new Job[num_of_slots];

            number_of_job_slots = num_of_slots;

            LinkedListConcurrentQueue<Job, int> new_free_queue(reinterpret_cast<Job *>(job_pool),
                                                               number_of_job_slots);
            free_queue = new_free_queue;

            free_queue.enqueueEntirePool();

            job_pool[0].state.store(JOB_STATE::DEAD_RINGER);

            for (int i = 1; i < number_of_job_slots; i++)
                job_pool[i].state.store(JOB_STATE::FREE_SLOT);

            // Setup priority queues

            for (int i = 0; i < number_of_priority_queues; i++)
                priority_queues[i] = LinkedListConcurrentQueue<Job, int>(
                    reinterpret_cast<Job *>(job_pool), number_of_job_slots);
        }
    }

    void JobDispatch::destroy()
    {
        if (job_pool) {
            delete[] job_pool;
        }
    }
} // namespace hive