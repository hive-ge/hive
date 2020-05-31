/**
 * Primitives and defines for use in debug mode.
 */

#pragma once

#include "include/primitive/core_type_information.hpp"
#include <iostream>
#include <string>


#ifndef NDEBUG
#define HIVE_DEBUG true
#endif

#define HIVE_FATAL_ERROR 1u

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
        /*std::cout << message << std::end;*/                                                      \
        throw((unsigned long long)StringHash64(message));                                          \
    }
/**
 * When in DEBUG mode, prints warning message to console, includes file and line location.
 *
 *
 */
#ifdef HIVE_DEBUG
#define HIVE_DEBUG_WARN(message)                                                                   \
    {                                                                                              \
        print message;                                                                             \
    }
#else
#define HIVE_DEBUG_WARN(message)
#endif


} // namespace hive
