/**
 * Primitives and defines for use in debug mode.
 */

#pragma once

#include "include/primitive/core_string_hash.hpp"
#include <iostream>
#include <string>


#ifndef NDEBUG
#define HIVE_DEBUG true
#endif

#ifdef HIVE_DEBUG
#define DEBUG_META(data) data;
#else
#define DEBUG_META(data) ;
#endif


namespace hive
{
    /* Basic Log will only output the error message to stdout*/
    void __LOG(std::string message, unsigned level = 0);

    /* Basic Errors will only output the error message to stdout or*/
    int __ERROR(std::string message);

    int __ERROR(std::string message, unsigned error_number);

    int __ERROR(std::string message, unsigned error_number, std::string filename,
                unsigned line_number);


    struct print_o {
        inline print_o & print(const unsigned line, const char * file)
        {
            std::cout << "In file " << file << "::" << line << ": \n";
            return *this;
        }
        template <class T> inline print_o & operator,(const T & data)
        {

            std::cout << data << std::endl;

            return *this;
        }
    };

    extern print_o print_e;

#define print print_e.print(__LINE__, __FILE__),

#define HIVE_DEBUG_ASSERT(expression, error_message)                                               \
    if (!(expression)) HIVE_ERROR(error_message)

#define HIVE_ERROR(message)                                                                        \
    {                                                                                              \
        print message;                                                                             \
        throw((unsigned long long)StringHash64(message));                                          \
    }
/**
 * When in DEBUG mode, prints warning message to console, includes file and line location.
 *
 *
 */
#ifdef HIVE_DEBUG


#ifdef HIVE_DEBUG_NO_WARN
#define HIVE_DEBUG_WARN(...)
#else
#define HIVE_DEBUG_WARN(...)                                                                       \
    {                                                                                              \
        print __VA_ARGS__;                                                                         \
    }
#endif

#define HIVE_DEBUG_MESSAGE(...)                                                                    \
    {                                                                                              \
        print __VA_ARGS__;                                                                         \
    }

#define HIVE_FATAL_ERROR(...)                                                                      \
    {                                                                                              \
        print __VA_ARGS__;                                                                         \
        throw 0;                                                                                   \
    }


#define STR(...) (#__VA_ARGS__)

#define HIVE_STATIC_WARN(string_arg) print #string_arg;

#else
#define HIVE_DEBUG_WARN(...)
#define HIVE_FATAL_ERROR(...)
#define HIVE_STATIC_WARN(string_arg)
#define HIVE_DEBUG_MESSAGE(...)
#define HIVE_MARK_CPU_COUNT_START(name)
#define HIVE_MARK_CPU_COUNT_MARK(name)
#endif

    inline uint64_t HIVE_DEBUG_getCPUCycles()
    {
        unsigned int lo, hi;
        __asm__ __volatile__("rdtscp" : "=a"(lo), "=d"(hi));
        return ((uint64_t)hi << 32) | lo;
    }

#define HIVE_MARK_CPU_COUNT_START(name)                                                            \
    hive_ull hive_cpu_clock_start_##name = HIVE_DEBUG_getCPUCycles();

#define HIVE_MARK_CPU_COUNT_MARK(name)                                                             \
    std::cout << #name                                                                             \
              << ": cycles counted: " << (HIVE_DEBUG_getCPUCycles() - hive_cpu_clock_start_##name) \
              << std::endl;


} // namespace hive
