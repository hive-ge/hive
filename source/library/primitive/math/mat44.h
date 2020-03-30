#pragma once

#include "./vec4.h"
#include <cmath>
#include <iostream>

namespace hive
{
    namespace math
    {

        template <class T, size_t s> class tgenVec
        {
            T v[s]{(T)0};
        };

        template <class T> class tmat44
        {
          public:
            union {
                tvec4<T> row1;
                struct {
                    T m1;
                    T m2;
                    T m3;
                    T m4;
                };
            };
            union {
                tvec4<T> row2;
                struct {
                    T m5;
                    T m6;
                    T m7;
                    T m8;
                };
            };
            union {
                tvec4<T> row3;
                struct {
                    T m9;
                    T m10;
                    T m11;
                    T m12;
                };
            };
            union {
                tvec4<T> row4;
                struct {
                    T m13;
                    T m14;
                    T m15;
                    T m16;
                };
            };

          public:
            /* Default Constructor will always initialize the Identity matrix */
            tmat44()
            {
                m1  = 1;
                m2  = 0;
                m3  = 0;
                m4  = 0;
                m5  = 0;
                m6  = 1;
                m7  = 0;
                m8  = 0;
                m9  = 0;
                m10 = 0;
                m11 = 1;
                m12 = 0;
                m13 = 0;
                m14 = 0;
                m15 = 0;
                m16 = 1;
            };

            tmat44(T _m1, T _m2, T _m3, T _m4, T _m5, T _m6, T _m7, T _m8, T _m9, T _m10, T _m11,
                   T _m12, T _m13, T _m14, T _m15, T _m16)
            {
                m1  = _m1;
                m2  = _m2;
                m3  = _m3;
                m4  = _m4;
                m5  = _m5;
                m6  = _m6;
                m7  = _m7;
                m8  = _m8;
                m9  = _m9;
                m10 = _m10;
                m11 = _m11;
                m12 = _m12;
                m13 = _m13;
                m14 = _m14;
                m15 = _m15;
                m16 = _m16;
            }

            tmat44(tvec4<T> a, tvec4<T> b, tvec4<T> c, tvec4<T> d)
            {
                row1 = a;
                row2 = b;
                row3 = c;
                row4 = d;
            }


            ~tmat44(){};
            // Creates a prospective projection matrix using Field of View, Aspect Ratio, and Near
            // and Far clipping values. Angles are in degrees.

            static tmat44<T> perspectiveProjectionMatrix(T field_of_view, T aspect_ratio,
                                                         T near_clip, T far_clip)
            {
                tmat44<T> Out_Mat44;

                T t = near_clip *
                      std::tan((field_of_view * ((T)3.141592653589793238462643383279 / 180.0f)) *
                               0.5f),
                  r = t * aspect_ratio, l = -r, b = -t;

                Out_Mat44.m1  = (near_clip * 2.f) / (r - l);
                Out_Mat44.m2  = 0;
                Out_Mat44.m3  = 0;
                Out_Mat44.m4  = 0;
                Out_Mat44.m5  = 0;
                Out_Mat44.m6  = (near_clip * 2.f) / (t - b);
                Out_Mat44.m7  = 0;
                Out_Mat44.m8  = 0;
                Out_Mat44.m9  = (r + l) / (r - l);
                Out_Mat44.m10 = (t + b) / (t - b);
                Out_Mat44.m11 = -(far_clip + near_clip) / (far_clip - near_clip);
                Out_Mat44.m12 = -1.f;
                Out_Mat44.m13 = 0;
                Out_Mat44.m14 = 0;
                Out_Mat44.m15 = (-2.f * far_clip * near_clip) / (far_clip - near_clip);
                Out_Mat44.m16 = 0;

                return Out_Mat44;
            }

            inline T * toArray() { return (T *)this; }

            friend inline tmat44<T> operator*(tmat44<T> & left, tmat44<T> & right)
            {
                return tmat44<T>(left.row1.dot(right.col1()), left.row1.dot(right.col2()),
                                 left.row1.dot(right.col3()), left.row1.dot(right.col4()),
                                 left.row2.dot(right.col1()), left.row2.dot(right.col2()),
                                 left.row2.dot(right.col3()), left.row2.dot(right.col4()),
                                 left.row3.dot(right.col1()), left.row3.dot(right.col2()),
                                 left.row3.dot(right.col3()), left.row3.dot(right.col4()),
                                 left.row4.dot(right.col1()), left.row4.dot(right.col2()),
                                 left.row4.dot(right.col3()), left.row4.dot(right.col4()));
            }

            friend inline tvec4<T> operator*(tmat44<T> & left, tvec4<T> & right) {}

            inline tvec4<T> col1() { return tvec4<T>(m1, m5, m9, m13); }

            inline tvec4<T> col2() { return tvec4<T>(m2, m6, m10, m14); }

            inline tvec4<T> col3() { return tvec4<T>(m3, m7, m11, m15); }

            inline tvec4<T> col4() { return tvec4<T>(m4, m8, m12, m16); }

            inline tmat44<T> transpose() { return mat44(col1(), col2(), col3(), col4()); }
        };

        typedef tmat44<float> mat44;
        typedef tmat44<float> mat44f;
        typedef tmat44<double> mat44d;
        typedef tmat44<int> mat44i;
    } // namespace math
} // namespace hive
