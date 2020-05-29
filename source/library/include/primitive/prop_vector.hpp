#pragma once

#include "include/primitive/core_math.hpp"

#include "include/primitive/core_prop.hpp"

#include "include/primitive/core_datafield.hpp"

#include <vector>

namespace hive
{
    using namespace hive::math;

    /**::HIVE DRONE_PROP::*/
    struct FloatVec3Prop : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, "PropVec3f");

        DataField<vec3> * data = nullptr;

        FloatVec3Prop() { data = DataField<vec3>::construct(); }

        ~FloatVec3Prop() { delete data; }
        // void removeVertex();
    };

    REGISTER_PROP(FloatVec3Prop)

    /**::HIVE DRONE_PROP::*/
    struct DoubleVec3Prop : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, "PropVec3d");

        DataField<vec3d> * data = nullptr;

        DoubleVec3Prop() { data = DataField<vec3d>::construct(); }

        ~DoubleVec3Prop() { delete data; }
        // void removeVertex();
    };

    REGISTER_PROP(DoubleVec3Prop)


    /**::HIVE DRONE_PROP::*/
    struct FloatProp : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, "PropFloat");

        DataField<float> * data = nullptr;

        FloatProp() : Prop() { data = DataField<float>::construct(); }

        ~FloatProp() { delete data; }

        void set(float d)
        {
            data->field = d;
            data->update();
        }
        // void removeVertex();
    };

    REGISTER_PROP(FloatProp)
} // namespace hive