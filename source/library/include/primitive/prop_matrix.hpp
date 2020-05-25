#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;

    //::HIVE DRONE_PROP
    struct DoubleMat44Prop : public Prop {

        static DoubleMat44Prop * construct() { return new DoubleMat44Prop(); }

        DoubleMat44Prop() : Prop("PROP_DMAT4", sizeof(DoubleMat44Prop))
        {
            mat44d * data = new mat44d;
            setData<mat44d>(data);
        }

        ~DoubleMat44Prop()
        {
            mat44d * data = getData<mat44d>();
            delete data;
        }
        // void removeVertex();
    };
} // namespace hive