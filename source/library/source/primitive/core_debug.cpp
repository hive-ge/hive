#include "include/hive.h"

namespace hive
{
    /* Basic Log will only output the error message to stdout*/
    void __LOG(std::string message, unsigned level) { std::cout << message << std::endl; };

    /* Basic Errors will only output the error message to stdout or*/
    int __ERROR(std::string message)
    {
        std::cout << message << std::endl;
        return 0;
    };

    int __ERROR(std::string message, unsigned error_number)
    {

        std::cout << "0x" << error_number << " : " << message << std::endl;

        HIVE_FATAL_ERROR(message);

        return 0;
    };

    int __ERROR(std::string message, unsigned error_number, std::string filename,
                unsigned line_number)
    {

        std::cout << "0x" << error_number << " : " << message << " " << filename << ":"
                  << line_number << std::endl;

        HIVE_FATAL_ERROR(message);

        return 0;
    };

    print_o print_e;
} // namespace hive
