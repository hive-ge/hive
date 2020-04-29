#pragma once

#include "./vec2.hpp"

#include "./line.hpp"

#include <algorithm>

namespace hive
{
    namespace math
    {
        template <class T> class tQuadraticBezier2D
        {
          public:
            union {
                T T_array[6];
                tvec2<T> vec_array[3];
                struct {
                    tvec2<T> p1, p2, p3;
                };
            };

          private:
            inline T posOnCurve(T t, T p1, T p2, T p3)
            {
                T ti = T(1.) - t;
                return ti * ti * p1 + 2.f * ti * t * p2 + t * t * p3;
            };

            inline tvec2<T> curvePoint(T t)
            {
                return tvec2<T>{posOnCurve(t, p1.x, p2.x, p3.x), posOnCurve(t, p1.y, p2.y, p3.y)};
            }

            inline T getTRootX()
            {
                T px1d = T(2) * (p2.x - p1.x);
                T px2d = T(2) * (p3.x - p2.x);
                T ad2  = -px1d + px2d;
                T bd2  = px1d;
                return -bd2 / ad2;
            }

            inline T getTRootY()
            {
                T py1d = T(2) * (p2.y - p1.y);
                T py2d = T(2) * (p3.y - p2.y);
                T ad1  = -py1d + py2d;
                T bd1  = py1d;
                return -bd1 / ad1;
            }

            inline unsigned intersections(T p1, T p2, T p3, T * intersections)
            {
                T a = p1 - T(2) * p2 + p3;
                T b = T(2) * (p2 - p1);
                T c = p1;

                if (b == T(0.0)) {
                    if (std::abs(a) < T(0.00000000005)) {
                        intersections[0] = (-c / b); // c / b;
                        return 1;
                    }

                    intersections[0] = ((-b - sqrt((b * b) - T(4.) * a * c)) / (T(2.) * a));
                    intersections[1] = ((-b + sqrt((b * b) - T(4.) * a * c)) / (T(2.) * a));

                    return 2;
                }

                return 0;
            }

            inline tQuadraticBezier2D<T> splitBoolean(T t, bool choice)
            {
                tQuadraticBezier2D<T> left, right;
                split(t, right, left);
                return (choice) ? right : left;
            }

          public:
            tQuadraticBezier2D(){};
            ~tQuadraticBezier2D(){};

            tQuadraticBezier2D(T x1, T y1, T x2, T y2, T x3, T y3)
                : p1(x1, y1), p2(x2, y2), p3(x3, y3){};
            tQuadraticBezier2D(tvec2<T> _p1, tvec2<T> _p2, tvec2<T> _p3)
                : p1(_p1), p2(_p2), p3(_p3){};

            inline tQuadraticBezier2D<T> reverse()
            {
                return tQuadraticBezier2D<T>(p3.x, p3.y, p2.x, p2.y, p1.x, p1.y);
            }

            friend tQuadraticBezier2D<T> operator*(T a, tQuadraticBezier2D<T> & b)
            {
                return tQuadraticBezier2D<T>(b.p1 * a, b.p2 * a, b.p3 * a);
            }

            friend tQuadraticBezier2D<T> operator*(tQuadraticBezier2D<T> & b, T a) { return a * b; }

            friend tQuadraticBezier2D<T> operator*(tQuadraticBezier2D<T> & b, tvec2<T> a)
            {
                return tQuadraticBezier2D<T>(b.p1 * a, b.p2 * a, b.p3 * a);
            }

            friend tQuadraticBezier2D<T> operator/(tQuadraticBezier2D<T> & b, tvec2<T> a)
            {
                return tQuadraticBezier2D<T>(b.p1 / a, b.p2 / a, b.p3 / a);
            }

            friend tQuadraticBezier2D<T> operator/(tQuadraticBezier2D<T> & b, T a)
            {
                return tQuadraticBezier2D<T>(b.p1 / a, b.p2 / a, b.p3 / a);
            }

            friend tQuadraticBezier2D<T> operator+(tQuadraticBezier2D<T> & b, tvec2<T> & a)
            {
                return tQuadraticBezier2D<T>(b.p1 + a, b.p2 + a, b.p3 + a);
            }

            friend tQuadraticBezier2D<T> operator+(tvec2<T> & a, tQuadraticBezier2D<T> & b)
            {
                return b + a;
            }

            friend tQuadraticBezier2D<T> operator-(tQuadraticBezier2D<T> & b, tvec2<T> & a)
            {
                return tQuadraticBezier2D<T>(b.p1 - a, b.p2 - a, b.p3 - a);
            }

            inline tvec2<T> tangent(T t)
            {
                tvec2<T> & _p1 = p2 - p1;
                tvec2<T> & _p2 = p3 - p2;
                return tvec2<T>{(1 - t) * _p1.x + t * _p2.x, (1 - t) * _p1.y + t * p2.y};
            }

            inline void split(T t, tQuadraticBezier2D<T> & left = tQuadraticBezier2D<T>(),
                              tQuadraticBezier2D<T> & right = tQuadraticBezier2D<T>())
            {
                left.p1  = p1;
                right.p3 = p3;
                left.p2  = ((T(1) - t) * p1 + t * p2);
                right.p2 = ((T(1) - t) * p2 + t * p3);
                right.p1 = ((T(1) - t) * left.p2 + t * right.p2);
                left.p3  = right.p1;
            }

            inline void roots(tvec2<T> & root_x, tvec2<T> & root_y)
            {
                root_x = curvePoint(getTRootX());
                root_y = curvePoint(getTRootY());
            }

            inline void rootsClamped(tvec2<T> & root_x, tvec2<T> & root_y)
            {
                T xr = getTRootX(), yr = getTRootY();
                root_x = (xr >= 0.f && xr <= 1.f) ? curvePoint(xr) : tvec2<T>(NAN, NAN);
                root_y = (yr >= 0.f && yr <= 1.f) ? curvePoint(yr) : tvec2<T>(NAN, NAN);
            }

            inline void boundingBox(tvec2<T> & min, tvec2<T> & max)
            {
                tvec2<T> root_x, root_y;
                rootsClamped(root_x, root_y);

                T groupA[] = {p1.x, p2.x, p3.x, 0.0f, 0.0f};
                T groupB[] = {p1.y, p2.y, p3.y, 0.0f, 0.0f};

                groupA[3] = (!root_x.isNAN()) ? root_x.x : INFINITY;
                groupA[4] = (!root_y.isNAN()) ? root_y.x : INFINITY;
                groupB[3] = (!root_x.isNAN()) ? root_x.y : INFINITY;
                groupB[4] = (!root_y.isNAN()) ? root_y.y : INFINITY;
                min.x     = *std::min_element(groupA, groupA + 5);
                min.y     = *std::min_element(groupB, groupB + 5);

                groupA[3] = (!root_x.isNAN()) ? root_x.x : -INFINITY;
                groupA[4] = (!root_y.isNAN()) ? root_y.x : -INFINITY;
                groupB[3] = (!root_x.isNAN()) ? root_x.y : -INFINITY;
                groupB[4] = (!root_y.isNAN()) ? root_y.y : -INFINITY;
                max.x     = *std::max_element(groupA, groupA + 5);
                max.y     = *std::max_element(groupB, groupB + 5);
            }

            inline tQuadraticBezier2D<T> rotate(T angle, tvec2<T> offset = tvec2<T>(0, 0))
            {
                tQuadraticBezier2D<T> new_curve;

                for (auto i = 0; i < 3; i++)
                    new_curve.vec_array[i] = (vec_array[i] - offset).rotate(angle) + offset;

                return new_curve;
            }

            inline tQuadraticBezier2D<T> lrsTransformCurve(tline2D<T> line)
            {
                return (rotate(-atan2(line.p2.y - line.p1.y, line.p2.x - line.p1.x), line.p1) -
                        line.p1) /
                       line.length();
            }

            // Make sure to pass a <T> array of at least length 2;
            inline unsigned xIntersections(T * array)
            {
                return intersections(p1.x, p2.x, p3.x, array);
            }

            inline unsigned yIntersections(T * array)
            {
                return intersections(p1.y, p2.y, p3.y, array);
            }

            inline unsigned char lineIntersections(tline2D<T> & line, T tn = T(0.0), T tp = T(0.0))
            {
                unsigned char results = 0;

                tline2D<T> last_new_line = line;

                tQuadraticBezier2D<T> rc = lrsTransformCurve(line);

                T y1 = rc.p1.y, y2 = rc.p2.y, y3 = rc.p3.y, x1 = rc.p1.x, x2 = rc.p2.x,
                  x3 = rc.p3.x, a = y1 - T(2) * y2 + y3, b = T(2) * (y2 - y1), c = y1;

                if (std::abs(a) < T(0.00000000000001)) {
                    tp = (-c / b);
                    tn = INFINITY;
                } else {
                    tn = ((-b - sqrt((b * b) - T(4) * a * c)) / (T(2) * a));
                    tp = ((-b + sqrt((b * b) - T(4) * a * c)) / (T(2) * a));
                }

                if (tn && tn >= T(.0005) && tn <= T(0.999)) {
                    T t = posOnCurve(tn, x1, x2, x3);
                    if (t >= T(0) && t <= T(1)) {
                        results++;
                    }
                }

                if (tp && tp >= T(0) && tp <= T(1)) {
                    T t = posOnCurve(tp, x1, x2, x3);
                    if (t >= T(0) && t <= T(1)) {
                        results++;
                    }
                }

                return results;
            }

            inline tline2D<T> lineSegment(T & T1, T & T2, T error_limit)
            {
                tvec2<T> vec1, vec2, center_c, center_l;
                T error = T(0), Tmid = T2;
                tline2D<T> line;

                vec1 = curvePoint(T1);

                do {
                    T2 = Tmid;

                    vec2 = curvePoint(T2);

                    line = tline2D<T>(vec1, vec2);

                    center_c = line.pointOnLine(T(0.5));

                    Tmid = T1 + (T2 - T1) * T(0.5);

                    center_l = curvePoint(Tmid);

                } while ((center_c - center_l).magnitude() > error_limit);

                return line;
            }

            inline bool lineSegments(tline2D<T> line, tQuadraticBezier2D<T> * curve_array,
                                     unsigned & curve_length, tline2D<T> * line_array,
                                     unsigned & line_length)
            {
                line_length  = 0;
                curve_length = 0;

                T tp, tn, divide = T(1);

                unsigned result = lineIntersections(line, tn, tp);

                bool HAVE_RESULT = false;

                if (result == 0) return HAVE_RESULT;

                tline2D<T> last_new_line = line;

                tQuadraticBezier2D<T> new_curve = *this, rc = lrsTransformCurve(line);

                T y1 = rc.p1.y, y2 = rc.p2.y, y3 = rc.p3.y, x1 = rc.p1.x, x2 = rc.p2.x,
                  x3 = rc.p3.x;

                tvec2<T> vector = line.p1 - line.p2;

                if (tn && tn >= T(0) && tn <= T(1)) {
                    T t    = posOnCurve(tn, x1, x2, x3);
                    divide = tn;
                    if (t >= T(0) && t <= T(1)) {
                        HAVE_RESULT = true;
                        bool BOOL   = y3 > T(0);
                        tline2D<T> new_line;
                        if (BOOL) {
                            curve_array[curve_length++] = new_curve.splitBoolean(tn, BOOL);
                            new_line           = tline2D<T>(line.p2, line.p1 - (vector * t));
                            last_new_line.p2.x = new_line.p1.x;
                            last_new_line.p2.y = new_line.p1.y;
                            // new_line.hot = true;
                        } else {
                            curve_array[curve_length++] = new_curve.splitBoolean(tn, !BOOL);
                            new_line           = tline2D<T>(line.p1, line.p1 - (vector * t));
                            last_new_line.p1.x = new_line.p2.x;
                            last_new_line.p1.y = new_line.p2.y;
                            // new_line.hot = true;
                        }
                        line_array[line_length++] = new_line;
                    }
                }

                if (tp && tp >= T(0) && tp <= T(1)) {
                    T t = posOnCurve(tp, x1, x2, x3);
                    if (t >= T(0) && t <= T(1)) {
                        HAVE_RESULT = true;
                        bool BOOL   = y1 > T(0);
                        tline2D<T> new_line;
                        if (BOOL) {
                            curve_array[curve_length++] = new_curve.splitBoolean(tp / divide, BOOL);
                            new_line           = tline2D<T>(line.p1, line.p1 - (vector * t));
                            last_new_line.p1.x = new_line.p2.x;
                            last_new_line.p1.y = new_line.p2.y;
                            // new_line.hot = true;
                        } else {
                            curve_array[curve_length++] = new_curve.splitBoolean(tp / divide, BOOL);
                            new_line           = tline2D<T>(line.p1 - (vector * t), line.p2);
                            last_new_line.p2.x = new_line.p1.x;
                            last_new_line.p2.y = new_line.p1.y;
                            // new_line.hot = true;
                        }
                        line_array[line_length++] = new_line;
                    }
                }

                if (HAVE_RESULT) line_array[line_length++] = last_new_line;

                return HAVE_RESULT;
            }
        };

        typedef tQuadraticBezier2D<float> QuadraticBezier2D;
        typedef tQuadraticBezier2D<float> QuadraticBezier2Df;
        typedef tQuadraticBezier2D<double> QuadraticBezier2Dd;
        typedef tQuadraticBezier2D<int> QuadraticBezier2Di;
    } // namespace math
} // namespace hive