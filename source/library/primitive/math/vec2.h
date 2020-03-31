#pragma once

#include <cmath>
#include <iostream>

namespace hive
{
    namespace math
    {
        template <class T> class tvec2
        {

          public: // Members
            T x = 0;
            T y = 0;

          public: // Member Functions
            // Constructors
            tvec2(){};
            ~tvec2(){};

            tvec2(T x_in, T y_in) : x(x_in), y(y_in){};

            tvec2(T f_in) : x(f_in), y(f_in){};

            // Overloaders

            inline friend std::ostream & operator<<(std::ostream & os, const tvec2<T> & vec)
            {
                os << "vec2-" << typeid(T).name() << "-[" << vec.x << "," << vec.y << "]";
                return os;
            }
            inline tvec2<T> & operator+=(tvec2<T> vr)
            {
                x += vr.x, y += vr.y;
                return *this;
            }

            inline tvec2<T> & operator-=(tvec2<T> vr)
            {
                x -= vr.x, y -= vr.y;
                return *this;
            }

            inline tvec2<T> & operator/=(tvec2<T> vr)
            {
                x /= vr.x, y /= vr.y;
                return *this;
            }


            inline tvec2<T> & operator*=(tvec2<T> vr)
            {
                x *= vr.x, y *= vr.y;
                return *this;
            }

            inline friend tvec2<T> operator-=(tvec2<T> & vl, tvec2<T> vr)
            {
                return tvec2<T>(vl.x + vr.x, vl.y + vr.y);
            }

            inline friend tvec2<T> operator-(tvec2<T> vl, tvec2<T> vr)
            {
                return tvec2<T>(vl.x - vr.x, vl.y - vr.y);
            }

            inline friend tvec2<T> operator+(tvec2<T> vl, tvec2<T> vr)
            {
                return tvec2<T>(vl.x + vr.x, vl.y + vr.y);
            }

            friend tvec2<T> operator/(tvec2<T> vl, tvec2<T> vr)
            {
                return tvec2<T>(vl.x / vr.x, vl.y / vr.y);
            }

            friend tvec2<T> operator*(tvec2<T> vl, tvec2<T> vr)
            {
                return tvec2<T>(vr.x * vl.x, vr.y * vl.y);
            }

            friend tvec2<T> operator*(T s, tvec2<T> vr) { return tvec2<T>(vr.x * s, vr.y * s); }

            friend tvec2<T> operator*(tvec2<T> vl, T s) { return s * vl; }

            friend tvec2<T> operator/(T s, tvec2<T> vr) { return tvec2<T>(vr.x / s, vr.y / s); }

            friend tvec2<T> operator/(tvec2<T> vl, T s) { return s / vl; }

            tvec2<T> & operator/=(T s)
            {
                x /= s;
                y /= s;
                return *this;
            }

            tvec2<T> & operator*=(T s)
            {
                x *= s;
                y *= s;
                return *this;
            }

            // Methods

            inline tvec2<T> negate() { return tvec2<T>(-x, -y); }

            inline tvec2<T> round() { return tvec2<T>(std::round(x), std::round(y)); }

            inline T dot(tvec2<T> v) { return x * v.x + y * v.y; }

            inline T magnitude() { return sqrt(dot(*this)); }

            inline tvec2<T> normalize() { return (*this) / magnitude(); }

            inline tvec2<T> rotate(T angle)
            {
                T fcos = cos(angle);
                T fsin = sin(angle);
                return tvec2<T>{x * fcos - y * fsin, x * fsin + y * fcos};
            }

            inline bool isNAN() { return x == NAN || y == NAN; }

            inline tvec2<T> yx() { return {y, x}; }
        };

        typedef tvec2<float> vec2;
        typedef tvec2<float> vec2f;
        typedef tvec2<double> vec2d;
        typedef tvec2<int> vec2i;
        typedef tvec2<unsigned int> vec2u;
    } // namespace math
} // namespace hive
