#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"

#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;

    //::HIVE DRONE_PROP
    struct FloatVec3Prop : Prop {

        vec3 * data = nullptr;

        static FloatVec3Prop * construct() { return new FloatVec3Prop(); }

        FloatVec3Prop() : Prop("PROP_FVEC3", sizeof(FloatVec3Prop)) { data = new vec3; }

        ~FloatVec3Prop() { delete data; }
        // void removeVertex();
    };

    //::HIVE DRONE_PROP
    struct DoubleVec3Prop : Prop {

        vec3d * data = nullptr;

        static DoubleVec3Prop * construct() { return new DoubleVec3Prop(); }

        DoubleVec3Prop() : Prop("PROP_DVEC3", sizeof(DoubleVec3Prop)) { data = new vec3d; }

        ~DoubleVec3Prop() { delete data; }
        // void removeVertex();
    };
} // namespace hive