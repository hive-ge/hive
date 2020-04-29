#pragma once
#include "./vec2.hpp"
#include <algorithm>

namespace hive
{
    namespace math
    {
        template <class T> class tline2D
        {
          public:
            union {
                T array[4];
                tvec2<T> vec_array[2];
                struct {
                    tvec2<T> p1, p2;
                };
            };

          public:
            tline2D(){};
            ~tline2D(){};

            tline2D(T p1x, T p1y, T p2x, T p2y) : p1(p1x, p1y), p2(p2x, p2y){};
            tline2D(tvec2<T> _p1, tvec2<T> _p2) : p1(_p1), p2(_p2) {}

            inline tline2D<T> flip() { return tline2D<T>(p2, p1); }

            inline T length() { return (p1 - p2).magnitude(); }

            inline tvec2<T> pointOnLine(T t) { return p1 + (p2 - p1) * t; }
        };

        typedef tline2D<float> line2D;
        typedef tline2D<float> line2Df;
        typedef tline2D<double> line2Dd;
        typedef tline2D<int> line2Di;
        typedef tline2D<unsigned> line2Du;
    } // namespace math
} // namespace hive