#pragma once
#include "vec4.hpp"
#include <cmath>
#include <iostream>

namespace hive
{
    namespace math
    {
        template <class T> class tquat
        {
            union {
                tvec4<T> vec4;
                struct {
                    T i;
                    T j;
                    T k;
                    T w;
                };
            };

          public:
            tquat(){};
            ~tquat(){};
        };

        typedef tquat<float> quat;
        typedef tquat<float> quatf;
        typedef tquat<double> quatd;
        typedef tquat<int> quati;
    } // namespace math
} // namespace hive