#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;

    /**::HIVE DRONE_PROP::*/
    struct DoubleMat44Prop : public Prop {

        CONSTRUCT_PROP(DoubleMat44Prop, "PropMat44d");

        mat44d * data = nullptr;

        DoubleMat44Prop() : Prop() { data = new mat44d; }

        ~DoubleMat44Prop() { delete data; }
        // void removeVertex();
    };

    REGISTER_PROP(DoubleMat44Prop);
} // namespace hive