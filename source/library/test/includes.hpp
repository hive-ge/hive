#pragma once

#include <iostream>
#include <stdio.h>


#define ASSERT(expression)                                                                         \
    if (!(expression)) {                                                                           \
        std::cout << "Test failed at line::" << __LINE__                                            \ 
                  << " with expression:["                                                          \
                  << #expression << "]" << std::endl;                                              \
        throw(-1);                                                                                 \
    }

#define SUCCESS() std::cout << "Test Passed" << std::endl;
