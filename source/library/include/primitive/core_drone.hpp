#pragma once

#include "include/primitive/core_drone_flag.hpp"
#include "include/primitive/core_indexed_pointer.hpp"
#include "include/primitive/core_memory_pool.hpp"
#include "include/primitive/core_type_information.hpp"
#include <shared_mutex>


namespace hive
{


    struct DroneData;
    struct Prop;

    typedef DroneFlagTemplate<0> DroneFlag;
    static const DroneFlagTemplate<1> DRONE_FLAG_NEED_UPDATE;

    /**
     * Primary container class for game objects
     */

    /**::HIVE DRONE_PROP::*/
    struct Drone {
        friend DroneDataPool;

        ADD_DRONE_DATA_REFERENCES(Drone)

        static const ushort DroneDataType = getDroneDataType("Drone");

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
      private:
        const DroneDataHandle_<DroneDataPool, Drone> ref;
        DronePropLU cache;

      public:
        /**
         * Linked list of observerables.
         */
        DroneDataHandle observation_chain = 0;

        DroneFlag flag;

      public:
        static Drone * construct();

        Drone(){};
        ~Drone() {}

        void connect(DroneDataHandle prop);

        void disconnect(DroneDataHandle prop);

        DroneDataHandle getProp(StringHash64 tag);

        DronePropLU getCache();

        const Ref getRef() { return DroneDataPool::getReference(this); }

        bool hasProp(const DronePropLU & lu) { return cache == lu; };

        template <drone_flag_primitive i> void setFlag(const DroneFlagTemplate<i> & new_flag)
        {
            flag += new_flag;
        };

        template <drone_flag_primitive i> void unsetFlag(const DroneFlagTemplate<i> & new_flag)
        {
            flag -= new_flag;
        };


        const DroneFlag getFlag(const DroneFlag & new_flag) { return flag; };
    };

    static_assert(sizeof(Drone) <= DroneDataPool::DroneDataStructSize,
                  "Prop size is greater than the pool allocation unit size");

} // namespace hive