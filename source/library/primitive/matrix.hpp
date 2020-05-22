#pragma once

#include <vector>

#include "gpu/opengl/gl.hpp"

#include "primitive/math/math.hpp"

#include "primitive/prop.hpp"

namespace hive
{
    using namespace hive::math;

    //::HIVE DRONE_PROP
    struct FloatMat22Prop : Prop {

        static FloatMat22Prop * construct() { return new FloatMat33Prop(); }

        FloatMat22Prop() : Prop(FloatMat22Prop, sizeof(FloatMat22Prop))
        {
            mat22 * data = new mat22;
            setData<mat22>(data);
        }

        ~FloatMat22Prop()
        {
            mat22 * data = getData<mat22>();
            delete data;
        }
        // void removeVertex();

        virtual void placeInGPU(int gpu_pointer);
    };

    // Move to GPU boss to handle merged structs.
    void FloatMat22Prop::placeInGPU(int gpu_pointer)
    {
        mat22 * data = getData<mat22>();
        if (data) glUniformMatrix2fv(gpu_pointer, 1, GL_FALSE, data);
    };

    //::HIVE DRONE_PROP
    struct DoubleMat22Prop : Prop {

        static DoubleMat22Prop * construct() { return new DoubleMat33Prop(); }

        DoubleMat22Prop() : Prop(DoubleMat22Prop, sizeof(DoubleMat22Prop))
        {
            vec3 * data = new vec3;
            setData<vec3>(data);
        }

        ~DoubleMat22Prop()
        {
            vec3 * data = getData<vec3>();
            delete data;
        }
        // void removeVertex();
    };

    //::HIVE DRONE_PROP
    struct FloatMat33Prop : Prop {

        static FloatMat33Prop * construct() { return new FloatMat33Prop(); }

        FloatMat33Prop() : Prop(FloatMat33Prop, sizeof(FloatMat33Prop))
        {
            vec3 * data = new vec3;
            setData<vec3>(data);
        }

        ~FloatMat33Prop()
        {
            vec3 * data = getData<vec3>();
            delete data;
        }
        // void removeVertex();
    };

    //::HIVE DRONE_PROP
    struct FloatMat44Prop : Prop {

        static FloatMat44Prop * construct() { return new FloatMat44Prop(); }

        FloatMat44Prop() : Prop(FloatMat44Prop, sizeof(FloatMat44Prop))
        {
            vec3d * data = new vec3d;
            setData<vec3d>(data);
        }

        ~FloatMat44Prop()
        {
            vec3d * data = getData<vec3d>();
            delete data;
        }
        // void removeVertex();
    };


    //::HIVE DRONE_PROP
    struct DoubleMat33Prop : Prop {

        static DoubleMat33Prop * construct() { return new DoubleMat33Prop(); }

        DoubleMat33Prop() : Prop(DoubleMat33Prop, sizeof(DoubleMat33Prop))
        {
            vec3 * data = new vec3;
            setData<vec3>(data);
        }

        ~DoubleMat33Prop()
        {
            vec3 * data = getData<vec3>();
            delete data;
        }
        // void removeVertex();
    };

    //::HIVE DRONE_PROP
    struct DoubleMat44Prop : Prop {

        static DoubleMat44Prop * construct() { return new DoubleMat44Prop(); }

        DoubleMat44Prop() : Prop(DoubleMat44Prop, sizeof(DoubleMat44Prop))
        {
            vec3d * data = new vec3d;
            setData<vec3d>(data);
        }

        ~DoubleMat44Prop()
        {
            vec3d * data = getData<vec3d>();
            delete data;
        }
        // void removeVertex();
    };
} // namespace hive