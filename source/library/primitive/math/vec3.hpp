#pragma once

#include "vec2.hpp"
#include <cmath>
#include <iostream>


namespace hive
{
    namespace math
    {
        template <class T> class tvec3
        {

          public: // Members
            T x = 0;
            T y = 0;
            T z = 0;

          public: // Member Functions
                  // Constructors
            tvec3(){};
            ~tvec3(){};

            tvec3(T x_in, T y_in, T z_in) : x(x_in), y(y_in), z(z_in){};

            tvec3(T f_in) : x(f_in), y(f_in), z(f_in){};

            // Overloaders

            friend std::ostream & operator<<(std::ostream & os, const tvec3<T> & vec)
            {
                os << "hive::vec3<" << typeid(T).name() << ">[" << vec.x << "," << vec.y << ","
                   << vec.z << "]";
                return os;
            }

            friend tvec3<T> operator-(tvec3<T> vl, tvec3<T> vr)
            {
                return tvec3<T>(vl.x - vr.x, vl.y - vr.y, vl.z - vr.z);
            }

            friend tvec3<T> operator+(tvec3<T> vl, tvec3<T> vr)
            {
                return tvec3<T>(vl.x + vr.x, vl.y + vr.y, vl.z + vr.z);
            }

            friend tvec3<T> operator*(T s, tvec3<T> vr)
            {
                return tvec3<T>(vr.x * s, vr.y * s, vr.z * s);
            }

            friend tvec3<T> operator*(tvec3<T> vl, T s) { return s * vl; }

            friend tvec3<T> operator*(tvec3<T> vl, tvec3<T> vr)
            {
                return tvec3<T>(vr.x * vl.x, vr.y * vl.y, vr.z * vl.z);
            }

            friend tvec3<T> operator/(T s, tvec3<T> vr)
            {
                return tvec3<T>(vr.x / s, vr.y / s, vr.z / s);
            }

            friend tvec3<T> operator/(tvec3<T> vl, T s) { return s / vl; }

            // Methods
            inline tvec3<T> negate() { return tvec3<T>(-x, -y, -z); }

            inline tvec3<T> round()
            {
                return tvec3<T>(std::round(x), std::round(y), std::round(z));
            }

            inline T dot(tvec3<T> v) { return x * v.x + y * v.y + z * v.z; }

            inline T magnitude() { return sqrt(dot(*this)); }

            inline tvec3<T> normalize() { return (*this) / magnitude(); }

            inline tvec3<T> lerp(tvec3<T> v, T t) { return (*this) + (v - (*this)) * t; }

            inline tvec3<T> cross(tvec3<T> v)
            {
                tvec3<T> a{y, z, x}, b{v.z, v.x, v.y}, c{z, x, y}, d{v.y, v.z, v.x};
                return (a * b) - (c * d);
            }

            inline tvec3<T> rotate(T angle)
            {
                T fcos = cos(angle);
                T fsin = sin(angle);
                return tvec3<T>{x * fcos - y * fsin, x * fsin + y * fcos};
            }

            inline bool isNAN() { return x == (T)NAN || y == (T)NAN || z == (T)NAN; }

            inline tvec2<T> xy() { return {x, y}; }
            inline tvec2<T> xz() { return {x, z}; }
            inline tvec2<T> yx() { return {y, x}; }
            inline tvec2<T> yz() { return {y, z}; }
            inline tvec2<T> zx() { return {z, x}; }
            inline tvec2<T> zy() { return {z, y}; }

            inline tvec3<T> xzy() { return {x, z, y}; }
            inline tvec3<T> yxz() { return {y, x, z}; }
            inline tvec3<T> yzx() { return {y, z, x}; }
            inline tvec3<T> zxy() { return {z, x, y}; }
            inline tvec3<T> zyx() { return {z, y, x}; }
        };

        typedef tvec3<float> vec3;
        typedef tvec3<float> vec3f;
        typedef tvec3<double> vec3d;
        typedef tvec3<int> vec3i;
        typedef tvec3<unsigned int> vec3u;
    } // namespace math
} // namespace hive
