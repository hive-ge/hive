#pragma once

#include <chrono>
#include <ctime>
#include <iostream>
#include <ratio>
#include <stdio.h>


#define ASSERT(expression)                                                                         \
    if (!(expression)) {                                                                           \
        std::cout << "Test failed at line::" << __LINE__                                            \ 
                  << " with expression:["                                                          \
                  << #expression << "]" << std::endl;                                              \
        throw(-1);                                                                                 \
    }

#define SUCCESS() std::cout << "Test Passed" << std::endl;

uint64_t getCPUCycles()
{
    unsigned int lo, hi;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

std::chrono::high_resolution_clock::time_point t1;

void timeMeasureStart() { t1 = std::chrono::high_resolution_clock::now(); }

void timeMeasureEndAndReport()
{
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> time_span =
        std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

    std::cout << "time: " << time_span.count() * 1000.0 * 1000.0 << "ns." << std::endl;
}
