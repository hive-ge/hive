#pragma once

#include "include/primitive/core_prop.hpp"
#include "include/primitive/core_typedef.hpp"


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

        void disconnect(Prop * prop);
    };
} // namespace hive