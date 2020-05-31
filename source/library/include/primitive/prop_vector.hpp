#pragma once


#include "include/primitive/core_math.hpp"

#include "include/primitive/core_prop.hpp"

#include <vector>

namespace hive
{
    using namespace hive::math;

    typedef DataPool::DataField<vec3> Vec3FloatDataField;
    typedef DataPool::DataField<vec3d> Vec3DoubleDataField;
    typedef DataPool::DataField<float> FloatDataField;

    /**::HIVE DRONE_PROP::*/
    struct FloatVec3Prop : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, "PropVec3f");

        Vec3FloatDataField::PTR data = nullptr;

        FloatVec3Prop() { data = general_data_pool.allocate<vec3>(); }

        ~FloatVec3Prop() {}
        // void removeVertex();
    };

    REGISTER_PROP(FloatVec3Prop)

    /**::HIVE DRONE_PROP::*/
    struct DoubleVec3Prop : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, "PropVec3d");

        Vec3DoubleDataField::PTR data = nullptr;

        DoubleVec3Prop() { data = general_data_pool.allocate<vec3d>(); }

        ~DoubleVec3Prop() {}
        // void removeVertex();
    };

    REGISTER_PROP(DoubleVec3Prop)


    /**::HIVE DRONE_PROP::*/
    struct FloatProp : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, "PropFloat");

        FloatDataField::PTR data = nullptr;

        FloatProp() : Prop() { data = general_data_pool.allocate<float>(); }

        ~FloatProp() {}

        void set(float d) { data = d; }
        // void removeVertex();
    };

    REGISTER_PROP(FloatProp)
} // namespace hivetimout