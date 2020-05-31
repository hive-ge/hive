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

        CONSTRUCT_PROP(DoubleMat44Prop, "mat44d");

        Mat44DoubleDataField::PTR data = nullptr;

        DoubleMat44Prop() : Prop() { data = general_data_pool.allocate<mat44d>(); }

        ~DoubleMat44Prop() {}
        // void removeVertex();
    };

    REGISTER_PROP(DoubleMat44Prop);
} // namespace hive