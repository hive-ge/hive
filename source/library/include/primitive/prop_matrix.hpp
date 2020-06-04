#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;

    typedef DataPool::DataField<mat44d> Mat44DoubleDataField;

    /**::HIVE DRONE_PROP::*/
    struct DoubleMat44Prop : public Prop {

        CONSTRUCT_PROP(DoubleMat44Prop, mat44d);

        Mat44DoubleDataField::PTR data = nullptr;

        DoubleMat44Prop() : Prop() { data = general_data_pool.allocate<mat44d>(); }

        ~DoubleMat44Prop() {}
        // void removeVertex();
    };

    REGISTER_PROP(DoubleMat44Prop);

    typedef DataPool::DataField<mat44> Mat44FloatPropDataField;

    /**::HIVE DRONE_PROP::*/
    struct Mat44FloatProp : public Prop {

        CONSTRUCT_PROP(Mat44FloatProp, mat44);

        Mat44FloatPropDataField::PTR data = nullptr;

        Mat44FloatProp() : Prop() { data = general_data_pool.allocate<mat44>(); }

        ~Mat44FloatProp() {}
        // void removeVertex();


        /**
         * t = tan(FOV / 2) * near
         * b = -t
         * l = -t * (w/h)
         * r = t * (w/h)
         *
         * [ 2*n/(r-l)      0       (r+l) / (r-l)        0  ]
         * [    0       2*n/(t-b)   (t+b) / (t-b)        0  ]
         * [    0           0       -(f+n) / (f-n)      -1  ]
         * [    0           0       -2 * f * n /  (f-n)  0  ]
         */

        void toPerspectiveProjectionMatrix(float height, float width, float fov_radians,
                                           float near_clip, float far_clip)
        {
            mat44 d = {};

            const float vr            = width / height;
            const float t             = std::tan(fov_radians * 0.5) * near_clip;
            const float b             = -t;
            const float r             = t * vr;
            const float l             = b * vr;
            const float rl_diff_ratio = 1 / (r - l);
            const float tb_diff_ratio = 1 / (t - b);
            const float fn_diff_ratio = 1 / (far_clip - near_clip);

            d.row1.x = 2 * near_clip * rl_diff_ratio;
            d.row2.y = 2 * near_clip * tb_diff_ratio;

            d.row3.x = (r + l) * rl_diff_ratio;
            d.row3.y = (b + t) * tb_diff_ratio;
            d.row3.z = (far_clip + near_clip) * fn_diff_ratio - 100.0;
            d.row3.w = -1;

            d.row4.z = -2 * far_clip * near_clip * fn_diff_ratio;

            data = d;

            const mat44 & e = data.raw();

            data.update();
        }

        void set(const mat44f & matrix)
        {
            data = matrix;

            data.update();
        }

        mat44f get() { return data.raw(); }
    };

    REGISTER_PROP(Mat44FloatProp);
} // namespace hive