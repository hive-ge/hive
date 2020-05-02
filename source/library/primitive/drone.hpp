#pragma once

#include "primitive/prop.hpp"
#include "primitive/typedef.hpp"


namespace hive
{
    struct DroneData;
    struct Drone;

    struct ID {
        struct {
            unsigned short padding;
            unsigned int instance_number;
            unsigned long long id;
        };
    };

    struct BigBadBoss;

    struct DroneData {
        friend Drone;
        unsigned long long flags = 0;
        Prop * props             = nullptr;
    };

    //::HIVE DRONE_PROP
    struct Drone {

        friend BigBadBoss;
        friend Prop;

      private:
        static BigBadBoss * boss;


        static inline void setBoss(BigBadBoss * ptr)
        {
            if (boss == nullptr) boss = ptr;
        };

        static inline BigBadBoss * getBoss() { return boss; };

      public:
        ID id;

      private:
        DroneData * data = nullptr;

      public:
        static Drone * construct();

        Drone(){};
        ~Drone() {}
        // Defined in boss.hpp;
        void signalUpdate();

        void connect(Prop * prop);

        void disconnect(Prop * prop)
        {
            DroneData & d = *data;

            if (prop == d.props) d.props = prop->next;

            if (prop->next) prop->next->prev = prop->prev;

            if (prop->prev) prop->prev->next = prop->next;

            prop->drone = nullptr;
        };
    };
} // namespace hive