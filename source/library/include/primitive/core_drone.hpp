#pragma once

#include "include/boss/boss.hpp"
#include "include/primitive/core_drone_flag.hpp"
#include "include/primitive/core_indexed_pointer.hpp"
#include "include/primitive/core_typedef.hpp"


namespace hive
{
    struct DroneData;
    struct Prop;

    typedef IndexedPointer<Drone, BigBadBoss> DronePointer;

    typedef DroneFlagTemplate<0> DroneFlag;
    static const DroneFlagTemplate<1> DRONE_FLAG_NEED_UPDATE;

    /**
     * Primary container class for game objects
     */
    //::HIVE DRONE_PROP
    struct Drone {

        friend BigBadBoss;
        friend Prop;

      private:
        static BigBadBoss * boss;

        static inline BigBadBoss * getBoss() { return boss; };

      private:
        /*
          Flag structure
          1 Can Render
          2 Needs Render Update
          3 Audio
          3 Needs Audio Update
          4 Needs
          3 Has Parent
        */
        const DronePointer this_ptr = -1;
        DronePointer parent         = -1;
        DronePointer next_sib       = -1;
        DronePointer first_child    = -1;

      public:
        Prop * props = nullptr;
        DroneFlag flag;

      public:
        static Drone * construct();

        Drone(DronePointer ptr) : this_ptr(ptr){};
        ~Drone() {}

        void connect(Prop * prop);

        void disconnect(Prop * prop);

        static inline void setBoss(BigBadBoss * ptr)
        {
            if (boss == nullptr) boss = ptr;
        };
    };

} // namespace hive