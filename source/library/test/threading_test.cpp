
#include "includes.hpp"
#include <hive.h>

using namespace hive;

hive::DataPool hive::general_data_pool(4096);

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
std::vector<int> data;
std::atomic<int> flag = {0};

void thread_1()
{
    while (flag.load(std::memory_order_acquire) == 2)
        ;
    // ata.push_back(42);
    flag.store(1, std::memory_order_release);
    std::cout << "1 done" << std::endl;
}

void thread_2()
{

    int expected = 2;
    // data.push_back(43);
    while (!flag.compare_exchange_strong(expected, 3, std::memory_order_release)) {
        expected = 2;
    }
    std::cout << "2 done" << std::endl;
}

void thread_3()
{
    while (flag.load(std::memory_order_acquire) != 1)
        ;
    assert(42 == 43); // will never fire
    std::cout << "3 done" << std::endl;
}

int main()
{
    std::thread b(thread_2);
    std::thread a(thread_1);
    std::thread c(thread_3);
    a.join();
    b.join();
    c.join();
}