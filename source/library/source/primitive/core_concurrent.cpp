/**
 * Primitives and defines for use in debug mode.
 */
#pragma once


#include <hive.h>

namespace hive
{

    JobDispatch::Job JobDispatch::job_pool[HIVE_JOB_POOL_SIZE];

    LinkedListConcurrentQueue<JobDispatch::Job, int> JobDispatch::free_queue;

    LinkedListConcurrentQueue<JobDispatch::Job, int> JobDispatch::priority_queues[10];

    int JobDispatch::next_free = 1;


    std::atomic<bool> ThreadRunner::KILL_SWITCHED    = false;
    thread_local unsigned ThreadRunner::thread_count = 0;
    thread_local int ThreadRunner::id                = 0;


    std::atomic_bool lock;

    void JobDispatch::promoteJobToPending(Job & job, unsigned priority)
    {
        int i, decrement;

        do {

            i = job.local_counter.load(std::memory_order_relaxed);

            decrement = i - 1;

            if (i == 0) HIVE_ERROR("Failed to run job. Job state has become corrupted");

        } while (
            !(job.local_counter.compare_exchange_strong(i, decrement, std::memory_order_relaxed)));

        job.state.store(JOB_STATE::PENDING);

        // TODO - Order job based on priority amongst other pending jobs.
        priority_queues[priority].enqueue(job, job);

        // If single threaded immediately execute this job.
        if (ThreadRunner::getNumberOfThreads() < 1) acquireJob();
    }


    JobDispatch::JOB & JobDispatch::acquireJobSlot()
    {
        thread_local unsigned exponential_sleep_count = 1;

        while (true) {

            JOB & job = free_queue.dequeue();

            if (job.state.load() == JOB_STATE::DEAD_RINGER) {

                // Try to acquire the cleanup lock and clean up all free slots
                // GRABBED LOCK

                bool locked = false;

                if (lock.compare_exchange_weak(locked, true)) {

                    JOB * first = nullptr;
                    JOB * last  = nullptr;

                    unsigned restored = 0;

                    for (int i = 0; i < HIVE_JOB_POOL_SIZE; i++) {

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
                        // std::cout << "Job queue has not starved " << restored << " "
                        //          << ThreadRunner::getID() << std::endl;
                    }

                    lock.store(false);

                    continue;
                }

                exponential_sleep_count = (exponential_sleep_count << 1) & 0xFFF;

                std::this_thread::sleep_for(std::chrono::microseconds(exponential_sleep_count));

                continue;
            }


            return job;
        }
    }

    void JobDispatch::runJob(Job & job)
    {

        job.local_counter.store(0, std::memory_order_relaxed);

        job.entry(job.data, job.data_size);

        if (job.destination_counter) {

            int i, decrement;

            do {

                i = job.destination_counter->load(std::memory_order_relaxed);

                decrement = i - 1;

            } while (!(job.destination_counter->compare_exchange_strong(
                i, decrement, std::memory_order_relaxed)));
        }

        job.state.store(JOB_STATE::COMPLETED, std::memory_order_release);
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

                        JOB_STATE A = JOB_STATE::PENDING;
                        // acquire this job_slot
                        if (!job.state.compare_exchange_weak(
                                A, JOB_STATE::ACTIVE, std::memory_order_acquire,
                                std::memory_order::memory_order_relaxed)) {
                            HIVE_ERROR("Unexpected job acquisition conflict");
                        }

                        // Run the job
                        try {
                            runJob(job);
                        } catch (int i) {
                            HIVE_ERROR("Failed to run job");
                        }

                        retireJobSlot(job);

                        return true;
                    }

                    break;
                }
            }
        }

        return false;
    }


    void JobDispatch::initialize()
    {
        LinkedListConcurrentQueue<Job, int> new_free_queue(reinterpret_cast<Job *>(job_pool),
                                                           HIVE_JOB_POOL_SIZE);
        free_queue = new_free_queue;

        free_queue.enqueueEntirePool();

        job_pool[0].state.store(JOB_STATE::DEAD_RINGER);

        for (int i = 1; i < HIVE_JOB_POOL_SIZE; i++) job_pool[i].state.store(JOB_STATE::FREE_SLOT);

        // Setup priority queues

        for (int i = 0; i < 10; i++)
            priority_queues[i] = LinkedListConcurrentQueue<Job, int>(
                reinterpret_cast<Job *>(job_pool), HIVE_JOB_POOL_SIZE);
    }
} // namespace hive