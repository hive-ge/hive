#pragma once


#include "include/primitive/core_math.hpp"

#include "include/primitive/core_prop.hpp"

#include <vector>

namespace hive
{
    using namespace hive::math;

    // Vectors
    typedef DataPool::DataField<vec4> Vec4FloatDataField;
    typedef DataPool::DataField<vec4d> Vec4DoubleDataField;
    typedef DataPool::DataField<vec3> Vec3FloatDataField;
    typedef DataPool::DataField<vec3d> Vec3DoubleDataField;
    typedef DataPool::DataField<vec2> Vec2FloatDataField;
    typedef DataPool::DataField<vec2d> Vec2DoubleDataField;

    typedef DataPool::DataField<quat> QuatDataField;
    typedef DataPool::DataField<quatd> QuatDoubleDataField;

    // Scalars
    typedef DataPool::DataField<float> FloatDataField;
    typedef DataPool::DataField<double> Float64DataField;
    typedef DataPool::DataField<unsigned> Uint32DataField;
    typedef DataPool::DataField<int> Int32DataField;
    typedef DataPool::DataField<unsigned long long int> Uint64DataField;
    typedef DataPool::DataField<long long int> Int64DataField;

    /**::HIVE DRONE_PROP::*/
    struct FloatVec3Prop : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, vec3);

        Vec3FloatDataField::PTR data = nullptr;

        FloatVec3Prop() { data = general_data_pool.allocate<vec3>(); }

        ~FloatVec3Prop() {}
        // void removeVertex();
    };

    REGISTER_PROP(FloatVec3Prop)

    /**::HIVE DRONE_PROP::*/
    struct DoubleVec3Prop : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, vec3d);

        Vec3DoubleDataField::PTR data = nullptr;

        DoubleVec3Prop() { data = general_data_pool.allocate<vec3d>(); }

        ~DoubleVec3Prop() {}
        // void removeVertex();
    };

    REGISTER_PROP(DoubleVec3Prop)


    /**::HIVE DRONE_PROP::*/
    struct FloatProp : Prop {

        CONSTRUCT_PROP(FloatVec3Prop, float32);

        FloatDataField::PTR data = nullptr;

        FloatProp() : Prop() { data = general_data_pool.allocate<float>(); }

        ~FloatProp() {}

        void set(float d) { data = d; }
        // void removeVertex();
    };


    REGISTER_PROP(FloatProp)


    /**::HIVE DRONE_PROP::*/
    struct Float64Prop : Prop {

        CONSTRUCT_PROP(Float64Prop, float64);

        Float64DataField::PTR data = nullptr;

        Float64Prop() : Prop() { data = general_data_pool.allocate<double>(); }

        ~Float64Prop() {}

        void set(double d) { data = d; }
        // void removeVertex();
    };

    REGISTER_PROP(Float64Prop)

    /**::HIVE DRONE_PROP::*/
    struct Int32Prop : Prop {

        CONSTRUCT_PROP(Int32Prop, int32);

        Int32DataField::PTR data = nullptr;

        Int32Prop() : Prop() { data = general_data_pool.allocate<int>(); }

        ~Int32Prop() {}

        void set(int v) { data = v; }
        // void removeVertex();
    };

    REGISTER_PROP(Int32Prop)

    /**::HIVE DRONE_PROP::*/
    struct Int64Prop : Prop {

        CONSTRUCT_PROP(Int64Prop, int64);

        Int64DataField::PTR data = nullptr;

        Int64Prop() : Prop() { data = general_data_pool.allocate<long long int>(); }

        ~Int64Prop() {}

        void set(long long int v) { data = v; }
        // void removeVertex();
    };

    REGISTER_PROP(Int64Prop)

    /**::HIVE DRONE_PROP::*/
    struct Uint32Prop : Prop {

        CONSTRUCT_PROP(Uint32Prop, uint32);

        Uint32DataField::PTR data = nullptr;

        Uint32Prop() : Prop() { data = general_data_pool.allocate<unsigned int>(); }

        ~Uint32Prop() {}

        void set(int v) { data = v; }
        // void removeVertex();
    };

    REGISTER_PROP(Int32Prop)

    /**::HIVE DRONE_PROP::*/
    struct Uint64Prop : Prop {

        CONSTRUCT_PROP(Uint64Prop, uint64);

        Uint64DataField::PTR data = nullptr;

        Uint64Prop() : Prop() { data = general_data_pool.allocate<unsigned long long int>(); }

        ~Uint64Prop() {}

        void set(unsigned long long int v) { data = v; }
        // void removeVertex();
    };

    REGISTER_PROP(Uint64Prop)

} // namespace hive