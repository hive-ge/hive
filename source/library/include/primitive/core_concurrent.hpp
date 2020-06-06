/**
 * Primitives and defines for use in debug mode.
 */
#pragma once

#include "include/primitive/core_debug.hpp"

#include <atomic>
#include <thread>
#include <typeinfo>


#define HIVE_JOB_POOL_SIZE 800
#define MAX_JOB_DEPENDENCIES 64
#define HIVE_CACHE_LINE_SIZE 64

namespace hive
{
    template <class T, class IndexVal> struct LinkedListConcurrentQueue {

        /**
         * Assumptions
         * 1. pool index [0] is assumed to be a null value of T, and is used
         * when fulfilling requests when queue is empty.
         *
         * 1. IndexVal is assumed to be an integer value capable of being assigned
         * [-1] to indicate null index locations.
         */

        static_assert(std::is_same<decltype(T::index), IndexVal>::value);
        static_assert(std::is_same<decltype(T::next), std::atomic<IndexVal>>::value);

      private:
        std::atomic<IndexVal> head = -1;
        std::atomic<IndexVal> tail = -1;

        T * pool;

        unsigned size;

      public:
        //-----------------------------------------------------------------------------//

        LinkedListConcurrentQueue() : pool(nullptr), size(0) {}

        LinkedListConcurrentQueue(T * pool, unsigned pool_size) : pool(pool), size(pool_size)
        {
            head.store(-1);
            tail.store(-1);
        }

        LinkedListConcurrentQueue(const LinkedListConcurrentQueue & queue)
        {
            pool = queue.pool;
            size = queue.size;
            head.store(queue.head.load());
            tail.store(queue.tail.load());
        }

        LinkedListConcurrentQueue & operator=(const LinkedListConcurrentQueue & queue)
        {
            pool = queue.pool;
            size = queue.size;
            head.store(queue.head.load());
            tail.store(queue.tail.load());

            return *this;
        }

        bool enqueue(T & obj_head, T & obj_tail)
        {

            obj_tail.next.store(-1, std::memory_order_relaxed);

            IndexVal obj_head_index = obj_head.index;
            IndexVal obj_tail_index = obj_tail.index;

            // set the output of the tail
            IndexVal tail_index = tail.load(std::memory_order_relaxed);

            if (obj_tail_index == 0)

                std::cout << "dequeing " << tail_index << " " << obj_head_index << std::endl;

            while (true) {

                IndexVal tail_index = tail.load(std::memory_order_relaxed);

                if (tail_index != -1) {

                    T & current_tail = pool[tail_index];

                    IndexVal tail_next_index = current_tail.next.load(std::memory_order_acq_rel);

                    if (tail_next_index == -1) {

                        if (!tail.compare_exchange_weak(tail_index, obj_tail_index,
                                                        std::memory_order_acq_rel))
                            continue;


                        if (!current_tail.next.compare_exchange_weak(
                                tail_next_index, obj_head_index, std::memory_order_acq_rel))

                            HIVE_ERROR("Object store FIFO corrupted: A ");
                        break;
                    }
                } else {
                    // Probably need to update head and tail

                    // Comfirm head is -1;

                    IndexVal head_index = head.load(std::memory_order_relaxed);

                    if (head == -1) {
                        if (!head.compare_exchange_weak(head_index, obj_head_index,
                                                        std::memory_order_release))
                            continue;
                        if (!tail.compare_exchange_weak(tail_index, obj_tail_index,
                                                        std::memory_order_release))
                            HIVE_ERROR("Object store FIFO corrupted: B ");
                        break;
                    }
                };
            }

            return true;
        }

        T & dequeue()
        {
            while (true) {

                IndexVal head_index = head.load(std::memory_order_relaxed);

                if (head_index == -1) return pool[0];

                T & current_head = pool[head_index];

                IndexVal next_head_index = current_head.next.load(std::memory_order_relaxed);

                if (next_head_index == -1) {

                    // if (!current_head.next.compare_exchange_weak(next_head_index, -1)) continue;

                    IndexVal tail_index = tail.load(std::memory_order_relaxed);

                    if (tail_index != head_index) continue;

                    if (!tail.compare_exchange_weak(tail_index, next_head_index,
                                                    std::memory_order_release))
                        continue;
                };

                if (!head.compare_exchange_weak(head_index, next_head_index)) continue;

                return current_head;
            }
        }

        void enqueueEntirePool()
        {

            for (int i = 0; i < size; i++) {
                pool[i].index = i;
                pool[i].next.store(i + 1, std::memory_order_relaxed);
            }

            pool[size - 1].next.store(-1, std::memory_order_relaxed);

            head.store(1);
            tail.store(size - 1);
        }
    }; // namespace hive

    typedef struct JobDispatch {

        typedef void (*JOB_ENTRY)(void * data, std::size_t param_data_length);

        enum JOB_STATE : unsigned {
            DEAD_RINGER,
            ACTIVE,
            PENDING,
            SETUP,
            COMPLETED,
            QUAISCENT_BLOCKED,
            FREE_SLOT
        };

        typedef struct JOB {
            //----------------------------
            JOB_ENTRY entry;
            //----------------------------
            void * data;
            //----------------------------
            unsigned data_size;
            std::atomic<int> local_counter;
            //----------------------------
            std::atomic<int> * destination_counter;
            //----------------------------
            std::atomic<JOB_STATE> state; // 4
            std::atomic<int> next;        // 4
            //----------------------------
            int index;        // 4
            unsigned unusedH; // time start
            //----------------------------
            unsigned unusedD; // time end
            unsigned unusedE; // priority
            //----------------------------
            unsigned unusedF;
            unsigned unusedG;

        } Job;

        static_assert(sizeof(JOB) <= HIVE_CACHE_LINE_SIZE, "Job is larger than cache line size.");

        static Job job_pool[HIVE_JOB_POOL_SIZE];

        static int next_free;

        const static Job dead_ringer;

        constexpr static int SETUP_COUNTER_START = 2;

        static LinkedListConcurrentQueue<Job, int> free_queue;
        static LinkedListConcurrentQueue<Job, int> priority_queues[10];

        //-----------------------------------------------------------------------------//

        typedef struct JOB_LANDING_ZONE {

            Job & destination;

            JOB_LANDING_ZONE(JOB_ENTRY lz, void * data, std::size_t data_size)
                : destination(acquireJobSlot())
            {
                if (destination.state == JOB_STATE::DEAD_RINGER)
                    HIVE_ERROR("No free job slots left!");

                destination.entry     = lz;
                destination.data      = data;
                destination.data_size = data_size;
                destination.local_counter.store(SETUP_COUNTER_START, std::memory_order_release);
            }

            void task(JOB_ENTRY lz, void * data, std::size_t data_size)
            {

                Job & job_slot = acquireJobSlot();

                if (job_slot.state == JOB_STATE::DEAD_RINGER) HIVE_ERROR("No free job slots left!");

                job_slot.destination_counter = &destination.local_counter;
                job_slot.entry               = lz;
                job_slot.data                = data;
                job_slot.data_size           = data_size;

                int i         = job_slot.destination_counter->load();
                int increment = i + 1;

                while (!(job_slot.destination_counter->compare_exchange_strong(
                    i, increment, std::memory_order_acq_rel))) {
                    i         = job_slot.destination_counter->load();
                    increment = i + 1;
                }

                job_slot.local_counter.store(SETUP_COUNTER_START, std::memory_order_relaxed);

                promoteJobToPending(job_slot, 0);
            }

            void execute() { promoteJobToPending(destination, 0); }
        } JobLZ;

        //-----------------------------------------------------------------------------//


      public:
        static void initialize();

        /**
         * Cycle through all jobs pending jobs and runs the highest priority job in the active
         * queues. Returns true if a job was run, false otherwise.
         */
        static bool acquireJob();

        static unsigned getFreeCount() noexcept
        {
            unsigned count = 0;
            for (int i = 0; i < HIVE_JOB_POOL_SIZE; i++)
                if (job_pool[i].state == JOB_STATE::FREE_SLOT) count++;
            ;
            return count;
        }
        //-----------------------------------------------------------------------------//

      private:
        static void runJob(Job & job);

        /**
         * Loads the given job into one of the priority queue slots.
         * If a call to ThreadRunner::getNumberOfThreads were to return 0
         * then promoting a job will also cause it to immediately run.
         */
        static void promoteJobToPending(Job & job, unsigned priority = 0);

        /**
         * Returns a free job_slot or the dead_ringer job_slot.
         */
        static void retireJobSlot(JOB & job) { job.state.store(JOB_STATE::COMPLETED); }

        /**
         * Returns a free job_slot or the dead_ringer job_slot.
         */
        static JOB & acquireJobSlot();
    } jobs;

    //#############################################################################//

    struct ThreadRunner {

      private:
        static std::atomic<bool> KILL_SWITCHED;

        static thread_local int id;

      public:
        static unsigned getID() { return id; }

        static unsigned getNumberOfThreads() { return thread_count; }

      private:
        static thread_local unsigned thread_count;

        std::thread ** threads;

        static void thread_runtime(int id, unsigned tc)
        {
            thread_local unsigned exponential_sleep_count = 1;
            thread_count                                  = tc;

            ThreadRunner::id = id;

            while (true) {
                // Greedily acquire jobs until there are none to acquire, then sleep for a short
                // durration.
                while (jobs::acquireJob()) exponential_sleep_count = 1;

                if (KILL_SWITCHED.load(std::memory_order_consume)) return;

                std::this_thread::sleep_for(
                    std::chrono::microseconds(exponential_sleep_count & 0xFFFF));

                exponential_sleep_count = exponential_sleep_count << 1;
            }
        }

      public:
        ThreadRunner(std::size_t number_of_threads = 2)
        {
            KILL_SWITCHED.store(false);

            thread_count = number_of_threads;

            threads = new std::thread *[number_of_threads];

            for (int i = 0; i < thread_count; i++) {
                threads[i] = new std::thread(thread_runtime, i + 1, thread_count);
            }
        };

        void detachThreads()
        {
            for (int i = 0; i < thread_count; i++) threads[i]->detach();
            thread_count = 0;
        }

        void join()
        {
            KILL_SWITCHED.store(true, std::memory_order_relaxed);

            for (int i = 0; i < thread_count; i++) threads[i]->join();

            thread_count = 0;
        }

        ~ThreadRunner()
        {
            join();

            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            delete[] threads;
        }
    };
} // namespace hive