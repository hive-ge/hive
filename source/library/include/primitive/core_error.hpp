#pragma once
#include "include/primitive/core_string_hash.hpp"
#include <iostream>
/**
 * Error throwing systems
 */
namespace hive
{
#define HIVE_ERROR(message)                                                                        \
    {                                                                                              \
        /*std::cout << message << std::end;*/                                                      \
        throw((unsigned long long)StringHash64(message));                                          \
    }


} // namespace hive