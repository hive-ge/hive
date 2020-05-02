#pragma once

#include "primitive/typedef.hpp"

// Property Types
#define NULL_PROP_TYPE 0
#define SPRITE_PROP_TYPE 1
#define TEXTURE_PROP_TYPE 2
#define PROGRAM_PROP_TYPE 3
#define MESH_PROP_TYPE 4
#define SHADER_PROGRAM_PROP_TYPE 5
#define IMAGE_PROP_TYPE 6
#define FLOAT_VEC3_PROP 7
#define DOUBLE_VEC3_PROP 8

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

        // ID used by bosses to enable data strucutre redirection.
        int ID = -1;

        inline void setID(int id) { ID = id; }

        inline int getID() { return ID; }

      public:
        static const ushort TYPE = NULL_PROP_TYPE;

        static Prop * construct() { return new Prop(); }

        DroneData * drone = nullptr;

      public:
        Prop * prev      = nullptr;
        Prop * next      = nullptr;
        ushort type      = 0;
        ushort byte_size = 0;

        Prop(const ushort _type = 0, const ushort size = sizeof(Prop))
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