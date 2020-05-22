#pragma once

#include <iostream>

#define HIVE_FATAL_ERROR 1u

namespace hive
{
    /* Basic Log will only output the error message to stdout*/
    extern void __LOG(std::string message, unsigned level = 0);

    /* Basic Errors will only output the error message to stdout or*/
    extern int __ERROR(std::string message);

    extern int __ERROR(std::string message, unsigned error_number);

    extern int __ERROR(std::string message, unsigned error_number, std::string filename,
                       unsigned line_number);
} // namespace hive