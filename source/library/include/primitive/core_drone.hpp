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
      public:
        /**
         * Linked list of attached props.
         */
        DroneDataHandle props = 0;

        /**
         * Linked list of observerables.
         */
        DroneDataHandle observation_chain = 0;

        /**
         * Set of flags to indicate different states of the Drone.
         *
         * Possibly unecessary in light of the cache property???
         */
        DroneFlag flags;

        /**
         * Flags indicating what prop types are attached to the drone.
         */
        DronePropLU cache;

      public:
        static Drone * construct();

        Drone(){};
        ~Drone() {}

        void connect(DroneDataHandle prop);

        void disconnect(DroneDataHandle prop);

        DroneDataHandle getProp(StringHash64 tag);
    };

    static_assert(offsetof(Drone, props) == 0, "Props reference is not at root of Drone");

    static_assert(sizeof(Drone) <= DroneDataPool::DroneDataStructSize,
                  "Prop size is greater than the pool allocation unit size");

} // namespace hive