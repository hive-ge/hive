
#pragma once

#include "include/boss/message_boss.hpp"
#include "include/primitive/core_drone.hpp"

namespace hive
{
    /**
     * Provides memory observation mechanism for
     * Drones.
     */
    struct DataChain {

        static const ushort DroneDataType = getDroneDataType("DroneDataChain");

        DroneDataHandle drone_link;
        DroneDataHandle chain_link;
        DroneDataHandle next_observer_link;
        DroneDataHandle prev_observer_link;
    };

    template <class D> struct DataField {

        static_assert(sizeof(D) >= 4, "DataField structures must be 4 bytes or more.");

        DroneDataHandle data_chain = 0;

        D field;

        DataField<D>() { new (&field) D(); }
        ~DataField<D>() {}

        static DataField<D> * construct() noexcept { return new DataField<D>; }

        static void destruct(DataField<D> * d) noexcept { delete d; }
        /**
         * Moves the data to a new buffer location or returns false if a move
         * is unsuccessful.
         */
        bool move(void * new_location) { return false; };

        /**
         * Removes header from data and returns pointer to the data field.
         * This effectivly makes the data static within it's buffer and all
         * dynamic memory management properties are lost, including the ability
         * to automatically update drone's when data field memory are changed.
         */
        void * degrade() { return nullptr; };

        void update(void * data)
        {
            memcpy(&field, data, sizeof(data));
            update();
        }

        void update() {}

        // get affected drones.
        void addDronePointer(DroneDataHandle _drone){};

        void removeDronePointer(DroneDataHandle drone){};

        unsigned getDroneRefCount() { return 0; };

        void getDroneRefs(DroneDataHandle[], unsigned array_size){};
    };
} // namespace hive