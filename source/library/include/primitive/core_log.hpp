#pragma once

#include <iostream>

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
} // namespace hive
