
#include "includes.hpp"
#include <atomic>
#include <hive.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace hive;

std::atomic<int> t = {0};
unsigned * test_datUM;

#define TEST_SIZE 5000

void task(void * data, std::size_t data_size)
{

    // convert data to correct structure.
    struct TestData {
        unsigned int counterA;
        unsigned int counterB;
    } * test_data = reinterpret_cast<TestData *>(data);
    int i         = 0;

    while (i++ < 50) {
        test_data->counterA = i + 2;
        test_data->counterA += test_data->counterB;
        test_data->counterB = test_data->counterA;
    }
    i                   = 10;
    test_data->counterB = i;
    while (i++ < 50) {
        test_data->counterA = i + 2;
        test_data->counterA += test_data->counterB;
        test_data->counterB = test_data->counterA;
    }
    i                   = 20;
    test_data->counterB = i;
    while (i++ < 50) {
        test_data->counterA = i + 2;
        test_data->counterA += test_data->counterB;
        test_data->counterB = test_data->counterA;
    }

    *((unsigned *)data + 15) = 2222222;
};

void lz(void * data, std::size_t data_size)
{
    std::cout << "Data: " << (hive_ull)data << std::endl;
    std::cout << "Datum: " << (hive_ull)test_datUM << std::endl;
    unsigned * d = reinterpret_cast<unsigned *>(data);

    for (int i = 0; i < TEST_SIZE; i++) {
        ASSERT(d[i * 16] == d[i * 16 + 1]);
        ASSERT(d[i * 16 + 15] == 2222222);
    }

    t.store(1, std::memory_order_release);
}


int main()
{
    const unsigned number_of_job_slots = 2;

    jobs::initialize(number_of_job_slots); // Job for the bigbadboss

    ThreadRunner thread_sys(4);

    test_datUM = new unsigned[TEST_SIZE * 16];

    for (int i = 0; i < TEST_SIZE; i++) {
        test_datUM[i * 16]      = i * 4;
        test_datUM[i * 16 + 1]  = i * 4 + 2;
        test_datUM[i * 16 + 2]  = i * 4 + 3;
        test_datUM[i * 16 + 3]  = i * 4 + 4;
        test_datUM[i * 16 + 4]  = i * 4 + 5;
        test_datUM[i * 16 + 5]  = i * 4 + 6;
        test_datUM[i * 16 + 6]  = i * 4 + 7;
        test_datUM[i * 16 + 7]  = i * 4 + 8;
        test_datUM[i * 16 + 8]  = i * 4 + 9;
        test_datUM[i * 16 + 9]  = i * 4 + 10;
        test_datUM[i * 16 + 10] = i * 4 + 11;
        test_datUM[i * 16 + 11] = i * 4 + 12;
        test_datUM[i * 16 + 12] = i * 4 + 13;
        test_datUM[i * 16 + 13] = i * 4 + 14;
        test_datUM[i * 16 + 14] = i * 4 + 15;
        test_datUM[i * 16 + 15] = i * 4 + 16;
    }
    timeMeasureStart();
    HIVE_MARK_CPU_COUNT_START(threadA)

    jobs::JobLZ job(lz, test_datUM, sizeof(test_datUM));

    for (int i = 0; i < TEST_SIZE; i++) job.task(task, test_datUM + i * 16, 64);

    job.execute();

    thread_sys.join();

    HIVE_MARK_CPU_COUNT_MARK(threadA)
    timeMeasureEndAndReport();

    ASSERT(t == 1);

    ASSERT(jobs::getFreeCount() == number_of_job_slots - 1);

    std::cout << "Free count: " << jobs::getFreeCount() << std::endl;

    thread_sys.kill();

    delete[] test_datUM;
}