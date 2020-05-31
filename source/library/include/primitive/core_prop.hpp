#pragma once

#include "include/primitive/core_drone.hpp"
#include "include/primitive/core_memory_pool.hpp"
#include "include/primitive/core_string_hash.hpp"
#include "include/primitive/core_type_information.hpp"
namespace hive
{
#define CONSTRUCT_PROP(prop, prop_id)                                                              \
    static const ushort DroneDataType       = getDroneDataType(prop_id);                           \
    static constexpr StringHash64 global_id = StringHash64(prop_id);                               \
    static prop * construct() { return DroneDataPool::createObject<prop>(); }                      \
    ADD_DRONE_DATA_REFERENCES(prop)


#define REGISTER_PROP(prop)                                                                        \
    static_assert(sizeof(prop) <= DroneDataPool::DroneDataStructSize,                              \
                  "##prop size is greater than the pool allocation unit size");                    \
    static_assert(prop::DroneDataType != getDroneDataType("PropRoot"),                             \
                  "Prop DroneDataType is not defined.");


    /**::HIVE DRONE_PROP::*/
    struct Prop {
        friend Boss;

      public:
        CONSTRUCT_PROP(Prop, "PropRoot")

        DroneDataHandle next  = 0;
        StringHash64 tag      = 0;
        DroneDataHandle drone = 0;

      protected:
        // char type = DroneDataType

      public:
        Prop() {}
        ~Prop() {}

        /**
         * Connects property to drone. Causes disconnection
         * from any existing drone. The prop is connected
         * to the end of the drone's property list.
         *
         * If the prop is already connected to the
         * the drone then this method will have no effect.
         */
        void connect(Drone::Ref drone);

        /**
         * Disconnects the property from it's drone
         * host. If the property has no host then this
         * method will have no effect.
         */
        void disconnect(Drone::Ref drone);

        inline StringHash64 getTagHash() { return tag; }

        inline std::string getTag() { return (std::string)tag; }

        inline void setTag(std::string string) { tag = string; /*onTagChange();*/ }

        // virtual void onTagChange(DronePointer drone) {}
    };

    static_assert(offsetof(Prop, next) == 0, "Prop next is not at root of Prop structure.");
    static_assert(sizeof(Prop) <= DroneDataPool::DroneDataStructSize,
                  "Prop size is greater than the pool allocation unit size");
} // namespace hive