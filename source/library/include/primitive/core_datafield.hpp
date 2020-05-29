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


    /**
     * Store for referencing drones from memory updates.
     *
     * Must be threadable.
     */
    struct DroneLookup {

        std::vector<std::vector<DronePointer>> drone_pointer_lu;

        unsigned addDrone(DronePointer pointer, unsigned index = 0) { return 0; }

        unsigned removeDrone(DronePointer pointer, unsigned index = 0) { return 0; }

        DronePointer getDronePointer(unsigned index = 0) { return 0; }

    } static DroneLookup;

    template <class D> struct DataField {

        static_assert(sizeof(D) >= 4, "DataField structures must be 4 bytes or more.");

        DroneDataHandle data_chain = 0;

        D field;

        DataField<D>() {}
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

        void update()
        {
            if (have_drone_pointer) {
                if (have_drone_lu) {

                } else {
                    sendMessage("Update Drone", "Drone Updating");
                }
            }
        }

        // get affected drones.
        void addDronePointer(DronePointer _drone)
        {
            if (!have_drone_pointer) {
                drone              = _drone;
                have_drone_pointer = true;
                return;
            }

            if (!have_drone_lu) {

                unsigned lu_index = DroneLookup.addDrone(drone);

                lu_index = DroneLookup.addDrone(_drone, lu_index);

                drone_lu_pointer = lu_index;

                have_drone_lu = true;

                return;
            }

            unsigned lu_index = DroneLookup.addDrone(_drone, lu_index);

            drone_lu_pointer = lu_index;
        };

        void removeDronePointer(DronePointer drone){};

        unsigned getDroneRefCount() { return 0; };

        void getDroneRefs(DronePointer[], unsigned array_size){};
    };
} // namespace hive