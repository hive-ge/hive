#pragma once

#include <cmath>
#include <iostream>

namespace hive
{
    namespace math
    {
        template <typename C> C random() { return ((C)rand())/((C)(RAND_MAX)) ; }

        template <typename C = float, class T> void makeRandom(T & obj, C min_val, C max_val)
        {
            const C diff = max_val - min_val;
            int size     = sizeof(T) / sizeof(C);
            C * array    = (C *)&obj;

            for (int i = 0; i < size; i++) array[i] = random<C>() * diff + min_val;
        }

        template <typename C = float, class T> void makeRandomClamped(T & obj, C min_val, C max_val)
        {
            const C diff = max_val - min_val;
            int size     = sizeof(T) / sizeof(C);
            C * array    = (C *)&obj;

            for (int i = 0; i < size; i++) array[i] = round(random<C>() * diff + min_val);
        }
    } // namespace math
} // namespace hive