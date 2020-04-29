#pragma once

#include "../typedef.hpp"

// Property Types
#define NULL_PROP_TYPE 0
#define SPRITE_PROP_TYPE 1
#define TEXTURE_PROP_TYPE 2
#define PROGRAM_PROP_TYPE 3

namespace hive
{
    struct Drone;

    //::HIVE DRONE_PROP
    struct Prop {

      public:
        static const ushort TYPE = NULL_PROP_TYPE;

        static Prop * construct() { return new Prop(); }

      public:
        Drone * drone    = nullptr;
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

    struct ID {
        struct {
            unsigned short padding;
            unsigned int instance_number;
            unsigned long long id;
        };
    };

    // Forward declare the big one.
    // class BigBadBoss;

    //::HIVE DRONE_PROP
    struct Drone {

        //  friend hive::BigBadBoss;

      private:
        static void * boss;

      public:
        ID id;

        static inline void setBoss(void * ptr)
        {
            if (boss == nullptr) boss = ptr;
        };

        static inline void * getBoss() { return boss; };

      public:
        unsigned long long flags = 0;

        Prop * props = nullptr;

        static Drone * construct();

      public:
        Drone(){};
        ~Drone() {}

        Drone * operator->();

        // Defined in boss.hpp;
        void signalUpdate();

        void connect(Prop * prop)
        {
            if (prop->drone) {
                if (prop->drone == this) return;
                prop->drone->disconnect(prop);
            }

            if (!props) {
                props = prop;
            } else {
                Prop * current = props;

                while (current->next) current = current->next;

                current->next = prop;
            }

            prop->drone = this;
        };

        void disconnect(Prop * prop)
        {
            if (prop == props) props = prop->next;

            if (prop->next) prop->next->prev = prop->prev;
            if (prop->prev) prop->prev->next = prop->next;

            prop->drone = nullptr;
        };
    };
} // namespace hive