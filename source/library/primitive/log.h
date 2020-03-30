#pragma once

#include <iostream>

#define HIVE_FATAL_ERROR 1u

namespace hive
{
    /* Basic Log will only output the error message to stdout*/
    extern void __LOG(std::string message, unsigned level = 0)
    {
        std::cout << message << std::endl;
    };

    /* Basic Errors will only output the error message to stdout or*/
    extern int __ERROR(std::string message)
    {
        std::cout << message << std::endl;
        return 0;
    };

    extern int __ERROR(std::string message, unsigned error_number)
    {

        std::cout << "0x" << error_number << " : " << message << std::endl;

        if (error_number & HIVE_FATAL_ERROR) throw - 1;

        return 0;
    };

    extern int __ERROR(std::string message, unsigned error_number, std::string filename,
                       unsigned line_number)
    {

        std::cout << "0x" << error_number << " : " << message << " " << filename << ":"
                  << line_number << std::endl;

        if (error_number & HIVE_FATAL_ERROR) throw - 1;

        return 0;
    };
} // namespace hive
