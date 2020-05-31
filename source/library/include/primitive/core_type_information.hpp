#pragma once

#include "include/primitive/core_const_info.hpp"
#include "include/primitive/core_string_hash.hpp"
#include <shared_mutex>

namespace hive
{

    constexpr unsigned getDroneDataType(StringHash64 type)
    {
        unsigned i = MEM_BARE_PROP_TYPE;
        switch (type) {

        case StringHash64("Drone"):
            return MEM_DRONE_TYPE;

        case StringHash64("DataChain"):
        case StringHash64("ViewChain"):
        case StringHash64("ObservationChain"):
            return MEM_DATA_CHAIN_TYPE;

        case StringHash64("PROP"):
            return MEM_BARE_PROP_TYPE;

        /****************************************
         * Properties
         ****************************************/
        default: // For static assertion to check that there are enough bits allocated in a
                 // DroneDataHandle for type information
            i++;

        // Complex Systems
        case StringHash64("PropMesh"):
            i++;
        case StringHash64("PropImage"):
            i++;
        case StringHash64("PropScript"):
            i++;
        case StringHash64("PropMaterial"):
            i++;
        case StringHash64("PropGPUProgram"):
            i++;
        case StringHash64("PropParent"):
            i++;
        case StringHash64("Animation"):
            i++;
        case StringHash64("Audio"):
            i++;

        // Numerics
        case StringHash64("PropMat44d"):
            i++;
        case StringHash64("PropMat44f"):
            i++;

        case StringHash64("PropMat33d"):
            i++;
        case StringHash64("PropMat33f"):
            i++;

        case StringHash64("PropQuatf"):
            i++;
        case StringHash64("PropQuatd"):
            i++;

        case StringHash64("PropVec3d"):
            i++;
        case StringHash64("PropVec3f"):
            i++;

        case StringHash64("PropVec2d"):
            i++;
        case StringHash64("PropVec2f"):
            i++;

        case StringHash64("PropFloat"):
            i++;
        case StringHash64("PropInt"):
            i++;
        case StringHash64("PropUint"):
            i++;

        // Attributes
        case StringHash64("PropRender"):
            i++;
        case StringHash64("PropTag"):
            i++;

            return i;
        }
    }

    static_assert(getDroneDataType("DEFAULT_VALUE_MAX") < MEM_MAX_ELEMENT_TYPE_COUNT,
                  "Field left for type information in DroneDataHandle is too small! Adjust bit "
                  "size of MAX_ELEMENT_BIT_COUNT to compensate for the number of property types.");


} // namespace hive