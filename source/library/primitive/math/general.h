#pragma once

#include "vec2.h"
#include <cmath>
#include <iostream>


namespace hive
{
    namespace math
    {
        template <typename C> C random() { return ((C)rand()) / ((C)(RAND_MAX)); }

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

        template <typename C = float>
        tvec2<C> lineIntersect2D(tvec2<C> vecA_start, tvec2<C> vecA_end, tvec2<C> vecB_start,
                                 tvec2<C> vecB_end)
        {
            C x1 = vecA_start.x;
            C y1 = vecA_start.y;
            C x2 = vecA_end.x;
            C y2 = vecA_end.y;
            C x3 = vecB_start.x;
            C y3 = vecB_start.y;
            C x4 = vecB_end.x;
            C y4 = vecB_end.y;

            float den = ((x1 - x2) * (y3 - y4) - (x3 - x4) * (y1 - y2));

            if (den < 0.0000001 && den > -0.0000001) return {(C)INFINITY};

            float t = ((x1 - x3) * (y3 - y4) - (x3 - x4) * (y1 - y3)) / den;
            float u = -(((x1 - x2) * (y1 - y3) - (x1 - x3) * (y1 - y2)) / den);

            if (-0.000001 > t && t < 1.000001 && -0.000001 > u && u < 1.000001)
                return vecA_start + (vecA_end - vecA_start) * t;

            return {(C)INFINITY};
        }
    } // namespace math
} // namespace hive