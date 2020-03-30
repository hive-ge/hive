#pragma once

#include "./vec3.h"
#include <cmath>
#include <iostream>


namespace hive
{
    namespace math
    {
        template <class T> class tvec4
        {

          public: // Members
            T x = 0;
            T y = 0;
            T z = 0;
            T w = 0;

          public: // Member Functions
                  // Constructors
            tvec4(){};
            ~tvec4(){};

            tvec4(T x_in, T y_in, T z_in, T w_in) : x(x_in), y(y_in), z(z_in), w(w_in){};

            tvec4(T f_in) : x(f_in), y(f_in), z(f_in), w(f_in){};

            // Overloaders

            friend std::ostream & operator<<(std::ostream & os, const tvec4<T> & vec)
            {
                os << "hive::vec4<" << typeid(T).name() << ">[" << vec.x << "," << vec.y << ","
                   << vec.z << "," << vec.w << "]";
                return os;
            }

            friend tvec4<T> operator-(tvec4<T> & vl, tvec4<T> & vr)
            {
                return tvec4<T>(vl.x - vr.x, vl.y - vr.y, vl.z - vr.z, vl.w - vr.w);
            }

            friend tvec4<T> operator+(tvec4<T> & vl, tvec4<T> & vr)
            {
                return tvec4<T>(vl.x + vr.x, vl.y + vr.y, vl.z + vr.z, vl.w + vr.w);
            }

            friend tvec4<T> & operator+=(tvec4<T> & vl, tvec4<T> vr)
            {
                vl.x += vr.x;
                vl.y += vr.y;
                vl.z += vr.z;
                vl.w += vr.w;

                return vl;
            }

            friend tvec4<T> & operator-=(tvec4<T> & vl, tvec4<T> vr)
            {
                vl.x -= vr.x;
                vl.y -= vr.y;
                vl.z -= vr.z;
                vl.w -= vr.w;

                return vl;
            }
            friend tvec4<T> operator*(T s, tvec4<T> & vr)
            {
                return tvec4<T>(vr.x * s, vr.y * s, vr.z * s, vr.w * s);
            }

            friend tvec4<T> operator*(tvec4<T> & vl, T s) { return s * vl; }

            friend tvec4<T> operator*(tvec4<T> & vl, tvec4<T> & vr)
            {
                return tvec4<T>(vr.x * vl.x, vr.y * vl.y, vr.z * vl.z, vr.w * vl.w);
            }

            friend tvec4<T> operator/(T s, tvec4<T> & vr)
            {
                return tvec4<T>(vr.x / s, vr.y / s, vr.z / s, vr.w / s);
            }

            friend tvec4<T> operator/(tvec4<T> & vl, T s) { return s / vl; }

            // Methods
            inline tvec4<T> negate() { return tvec4<T>(-x, -y, -z, -w); }

            inline tvec4<T> round()
            {
                return tvec4<T>(std::round(x), std::round(y), std::round(z), std::round(w));
            }

            inline T dot(tvec4<T> v) { return x * v.x + y * v.y + z * v.z + w * v.w; }

            inline T magnitude() { return sqrt(dot(*this)); }

            inline tvec4<T> normalize() { return (*this) / magnitude(); }

            inline tvec4<T> lerp(tvec4<T> & v, T t) { return (*this) + (v - (*this)) * t; }

            inline tvec4<T> cross(tvec4<T> & v)
            {
                tvec4<T> a{y, z, x, 1}, b{v.z, v.x, v.y, 1}, c{z, x, y, 1}, d{v.y, v.z, v.x, 1};
                return (a * b) - (c * d);
            }

            inline tvec4<T> rotate(T angle)
            {
                T fcos = cos(angle);
                T fsin = sin(angle);
                return tvec4<T>{x * fcos - y * fsin, x * fsin + y * fcos};
            }

            inline bool isNAN() { return x == (T)NAN || y == (T)NAN || z == (T)NAN; }
        };

        typedef tvec4<float> vec4;
        typedef tvec4<float> vec4f;
        typedef tvec4<double> vec4d;
        typedef tvec4<int> vec4i;
        typedef tvec4<unsigned int> vec4u;
    } // namespace math
} // namespace hive
