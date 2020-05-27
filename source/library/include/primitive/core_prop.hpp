#pragma once

#include "include/primitive/core_drone.hpp"
#include "include/primitive/core_string_hash.hpp"
#include "include/primitive/core_typedef.hpp"

// Property Types
#define _NULL_PROP_TYPE 0
#define _SPRITE_PROP_TYPE 1
#define _TEXTURE_PROP_TYPE 2
#define _PROGRAM_PROP_TYPE 3
#define _MESH_PROP_TYPE 4
#define _SHADER_PROGRAM_PROP_TYPE 5
#define _IMAGE_PROP_TYPE 6
#define _FLOAT_VEC3_PROP 7
#define _DOUBLE_VEC3_PROP 8
#define _FloatMat22Prop 9
#define _DoubleMat22Prop 10
#define _FloatMat33Prop 11
#define _FloatMat44Prop 12
#define _DoubleMat33Prop 13
#define _DoubleMat44Prop 14

namespace hive
{

    //::HIVE DRONE_PROP
    struct Prop {
        friend Boss;

      public:
        static const ushort TYPE = _NULL_PROP_TYPE;

        static Prop * construct() { return new Prop("DEFAULT_PROP_DO_NOT_USE"); }


      public:
        const StringHash64 type = "";
        StringHash64 tag        = "";
        Prop * prev             = nullptr;
        Prop * next             = nullptr;
        DronePointer drone      = 0;

        Prop(const StringHash64 _type, const ushort size) : type(_type) {}
        Prop(const StringHash64 _type) : type(_type) {}

        ~Prop() { disconnect(); }

        /**
         * Connects property to drone. Causes disconnection
         * from any existing drone. The prop is connected
         * to the end of the drone's property list.
         *
         * If the prop is already connected to the
         * the drone then this method will have no effect.
         */
        void connect(DronePointer drone);

        virtual void onConnect(DronePointer drone) {}
        /**
         * Disconnects the property from it's drone
         * host. If the property has no host then this
         * method will have no effect.
         */
        void disconnect();
    };
} // namespace hive