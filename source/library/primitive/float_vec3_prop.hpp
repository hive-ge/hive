#pragma once

#include <vector>

#include "primitive/math/math.hpp"

#include "primitive/prop.hpp"

namespace hive
{
    using namespace hive::math;

    //::HIVE DRONE_PROP
    struct FloatVec3Prop : Prop {

        static FloatVec3Prop * construct() { return new FloatVec3Prop(); }

        FloatVec3Prop() : Prop(FLOAT_VEC3_PROP, sizeof(FloatVec3Prop))
        {
            vec3 * data = new vec3;
            setData<vec3>(data);
        }

        ~FloatVec3Prop()
        {
            vec3 * data = getData<vec3>();
            delete data;
        }
        // void removeVertex();
    };

    //::HIVE DRONE_PROP
    struct DoubleVec3Prop : Prop {

        static DoubleVec3Prop * construct() { return new DoubleVec3Prop(); }

        DoubleVec3Prop() : Prop(DOUBLE_VEC3_PROP, sizeof(DoubleVec3Prop))
        {
            vec3d * data = new vec3d;
            setData<vec3d>(data);
        }

        ~DoubleVec3Prop()
        {
            vec3d * data = getData<vec3d>();
            delete data;
        }
        // void removeVertex();
    };
} // namespace hive