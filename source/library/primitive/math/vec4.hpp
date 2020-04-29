#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
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

            tvec4<T> & operator/=(T s)
            {
                x /= s;
                y /= s;
                z /= s;
                w /= s;
                return *this;
            }

            tvec4<T> & operator*=(T s)
            {
                x *= s;
                y *= s;
                z *= s;
                w *= s;
                return *this;
            }

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

            inline tvec2<T> xy() { return {x, y}; }
            inline tvec2<T> xz() { return {x, z}; }
            inline tvec2<T> xw() { return {x, w}; }
            inline tvec2<T> yx() { return {y, x}; }
            inline tvec2<T> yz() { return {y, z}; }
            inline tvec2<T> yw() { return {y, w}; }
            inline tvec2<T> zx() { return {z, x}; }
            inline tvec2<T> zy() { return {z, y}; }
            inline tvec2<T> zw() { return {z, w}; }
            inline tvec2<T> wx() { return {w, x}; }
            inline tvec2<T> wy() { return {w, y}; }
            inline tvec2<T> wz() { return {w, z}; }

            inline tvec3<T> xyz() { return {x, y, z}; }
            inline tvec3<T> xyw() { return {x, y, w}; }
            inline tvec3<T> xzy() { return {x, z, y}; }
            inline tvec3<T> xzw() { return {x, z, w}; }
            inline tvec3<T> xwy() { return {x, w, y}; }
            inline tvec3<T> xwz() { return {x, w, z}; }

            inline tvec3<T> yxz() { return {y, x, z}; }
            inline tvec3<T> yxw() { return {y, x, w}; }
            inline tvec3<T> yzx() { return {y, z, x}; }
            inline tvec3<T> yzw() { return {y, z, w}; }
            inline tvec3<T> ywx() { return {y, w, x}; }
            inline tvec3<T> ywz() { return {y, w, z}; }

            inline tvec3<T> zxy() { return {z, x, y}; }
            inline tvec3<T> zxw() { return {z, x, w}; }
            inline tvec3<T> zyx() { return {z, y, x}; }
            inline tvec3<T> zyw() { return {z, y, w}; }
            inline tvec3<T> zwx() { return {z, w, x}; }
            inline tvec3<T> zwy() { return {z, w, y}; }

            inline tvec3<T> wxy() { return {w, x, y}; }
            inline tvec3<T> wxz() { return {w, x, z}; }
            inline tvec3<T> wyx() { return {w, y, x}; }
            inline tvec3<T> wyz() { return {w, y, z}; }
            inline tvec3<T> wzx() { return {w, z, x}; }
            inline tvec3<T> wzy() { return {w, z, y}; }

            inline tvec4<T> xzyw() { return {x, z, y, w}; }
            inline tvec4<T> yxzw() { return {y, x, z, w}; }
            inline tvec4<T> yzxw() { return {y, z, x, w}; }
            inline tvec4<T> zxyw() { return {z, x, y, w}; }
            inline tvec4<T> zyxw() { return {z, y, x, w}; }

            inline tvec4<T> xywz() { return {x, y, w, z}; }
            inline tvec4<T> xwyz() { return {x, w, y, z}; }
            inline tvec4<T> yxwz() { return {y, x, w, z}; }
            inline tvec4<T> ywxz() { return {y, w, x, z}; }
            inline tvec4<T> wxyz() { return {w, x, y, z}; }
            inline tvec4<T> wyxz() { return {w, y, x, z}; }

            inline tvec4<T> xzwy() { return {x, z, w, y}; }
            inline tvec4<T> xwzy() { return {x, w, z, y}; }
            inline tvec4<T> zxwy() { return {z, x, w, y}; }
            inline tvec4<T> zwxy() { return {z, w, x, y}; }
            inline tvec4<T> wxzy() { return {w, x, z, y}; }
            inline tvec4<T> wzxy() { return {w, z, x, y}; }

            inline tvec4<T> yzwx() { return {y, z, w, x}; }
            inline tvec4<T> ywzx() { return {y, w, z, x}; }
            inline tvec4<T> zywx() { return {z, y, w, x}; }
            inline tvec4<T> zwyx() { return {z, w, y, x}; }
            inline tvec4<T> wyzx() { return {w, y, z, x}; }
            inline tvec4<T> wzyx() { return {w, z, y, x}; }
        };

        typedef tvec4<float> vec4;
        typedef tvec4<float> vec4f;
        typedef tvec4<double> vec4d;
        typedef tvec4<int> vec4i;
        typedef tvec4<unsigned int> vec4u;
    } // namespace math
} // namespace hive
