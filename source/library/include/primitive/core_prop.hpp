#pragma once

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
        friend Drone;
        friend Boss;

      protected:
        // Generic pointer for custom data structures.
        void * data = nullptr;

        template <class T> inline T * getData() { return static_cast<T *>(data); }

        template <class T> inline void setData(T * d) { data = static_cast<void *>(d); }

        // ID used by bosses to enable data structure redirection.
        int ID = -1;

        inline void setID(int id) { ID = id; }

        inline int getID() { return ID; }

      public:
        static const ushort TYPE = _NULL_PROP_TYPE;

        static Prop * construct() { return new Prop(); }

        DroneData * drone = nullptr;

      public:
        StringHash64 type = "";
        Prop * prev       = nullptr;
        Prop * next       = nullptr;
        ushort byte_size  = 0;

        Prop(const std::string _type = "", const ushort size = sizeof(Prop))
            : type(_type), byte_size(size)
        {
        }

        ~Prop() { disconnect(); }

        /**
         * Connects property to drone. Causes disconnection
         * from any existing drone. The prop is connected
         * to the end of the drone's property list.
         *
         * If the prop is already connected to the
         * the drone then this method will have no effect.
         */
        void connect(hive::Drone *);

        /**
         * Disconnects the property from it's drone
         * host. If the property has no host then this
         * method will have no effect.
         */
        void disconnect();
    };
} // namespace hive