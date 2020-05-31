#pragma once

#include "core_debug.hpp"
#include "include/primitive/core_string_hash.hpp"
#include <shared_mutex>

/**
 * Type Information
 */
namespace hive
{
    /**
     * Information for DroneDataHandles and DroneDataPool
     */
    constexpr unsigned MEM_MAX_ELEMENT_BIT_COUNT = 26;
    constexpr unsigned MEM_MAX_ELEMENT_SIZE      = (1u << MEM_MAX_ELEMENT_BIT_COUNT) - 2u;
    constexpr unsigned MEM_MAX_INDEX             = (1u << MEM_MAX_ELEMENT_BIT_COUNT) - 1u;
    constexpr unsigned MEM_INDEX_MASK            = ((1u << MEM_MAX_ELEMENT_BIT_COUNT) - 1u);
    constexpr unsigned MEM_TYPE_MASK             = ~MEM_INDEX_MASK;
    constexpr unsigned MEM_MAX_ELEMENT_TYPE_COUNT =
        (MEM_TYPE_MASK >> MEM_MAX_ELEMENT_BIT_COUNT) + 1u;

    // Specific type information for a data chain element.
    constexpr unsigned MEM_BARE_PROP_TYPE  = 3;
    constexpr unsigned MEM_DRONE_TYPE      = 1;
    constexpr unsigned MEM_DATA_CHAIN_TYPE = 2;

    // Forward Declarations;
    struct DroneData;
    struct Drone;
    class Boss;

    enum HIVE_RESULT : char {
        SUCCESS,
        READ_ERROR,
        WRITE_ERROR,
        OVERFLOW_ERROR,
        INVALID_ARGUMENT_ERROR,
    };

    typedef unsigned short ushort;
    typedef unsigned char ubyte;
    typedef char byte;
    typedef unsigned int uint;
    typedef hive_string_hash_64_ull u_ull;
    typedef hive_string_hash_64_ull hive_ull;

#define SetPropIndexLookup(prop)                                                                   \
    case StringHash64(#prop):                                                                      \
        i++;

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
            SetPropIndexLookup(mesh);
            SetPropIndexLookup(image);
            SetPropIndexLookup(grid);
            SetPropIndexLookup(script);
            SetPropIndexLookup(material);
            SetPropIndexLookup(gpu_program);
            SetPropIndexLookup(parent);
            SetPropIndexLookup(animation);
            SetPropIndexLookup(audio);

            // Numerics
            SetPropIndexLookup(mat44d);
            SetPropIndexLookup(mat44);
            SetPropIndexLookup(mat33d);
            SetPropIndexLookup(mat33);
            SetPropIndexLookup(quat);
            SetPropIndexLookup(quatd);
            SetPropIndexLookup(vec3d);
            SetPropIndexLookup(vec3);
            SetPropIndexLookup(vec2d);
            SetPropIndexLookup(vec2);
            SetPropIndexLookup(float32);
            SetPropIndexLookup(float64);
            SetPropIndexLookup(int32);
            SetPropIndexLookup(uint32);
            SetPropIndexLookup(int64);
            SetPropIndexLookup(uint64);

            // Attributes
            SetPropIndexLookup(render);
            SetPropIndexLookup(tag);

            return i;
        }

        return 0;
    }

#define setDroneLUStaticBITFlag(prop) prop = 1 << getDroneDataType(#prop)

    struct DronePropLU {
        enum : hive_ull {
            setDroneLUStaticBITFlag(material),
            setDroneLUStaticBITFlag(mesh),
            setDroneLUStaticBITFlag(grid),
            setDroneLUStaticBITFlag(image),
            setDroneLUStaticBITFlag(script),
            setDroneLUStaticBITFlag(gpu_program),
            setDroneLUStaticBITFlag(parent),
            setDroneLUStaticBITFlag(animation),
            setDroneLUStaticBITFlag(audio),
            setDroneLUStaticBITFlag(mat44d),
            setDroneLUStaticBITFlag(mat44),
            setDroneLUStaticBITFlag(mat33d),
            setDroneLUStaticBITFlag(mat33),
            setDroneLUStaticBITFlag(quat),
            setDroneLUStaticBITFlag(quatd),
            setDroneLUStaticBITFlag(vec3d),
            setDroneLUStaticBITFlag(vec3),
            setDroneLUStaticBITFlag(vec2d),
            setDroneLUStaticBITFlag(vec2),
            setDroneLUStaticBITFlag(float32),
            setDroneLUStaticBITFlag(float64),
            setDroneLUStaticBITFlag(int32),
            setDroneLUStaticBITFlag(uint32),
            setDroneLUStaticBITFlag(int64),
            setDroneLUStaticBITFlag(uint64),
            setDroneLUStaticBITFlag(render),
            setDroneLUStaticBITFlag(tag)
        };

        hive_ull flag;

        constexpr DronePropLU() : DronePropLU((unsigned int)0) {}

        constexpr DronePropLU(const unsigned int val) : flag(1ull << val) {}

        constexpr DronePropLU(const hive_ull val) : flag(val) {}

        constexpr DronePropLU & operator+=(const hive_ull val)
        {
            flag |= val;
            return *this;
        }

        constexpr DronePropLU & operator-=(const hive_ull val)
        {
            flag &= ~val;
            return *this;
        }


        constexpr DronePropLU & operator=(const hive_ull val)
        {
            flag = val;
            return *this;
        }


        constexpr DronePropLU & operator+=(const unsigned val)
        {
            flag += 1ull << val;
            return *this;
        }

        constexpr DronePropLU & operator-=(const unsigned val)
        {
            flag -= 1ull << val;
            return *this;
        }


        constexpr DronePropLU & operator=(const unsigned val)
        {
            flag = 1ull << val;
            return *this;
        }

        constexpr bool operator==(const hive_ull val) const { return (flag & val) == val; }

        constexpr bool operator!=(const hive_ull val) const { return (flag & val) != val; }

        constexpr bool operator==(const unsigned val) const { return flag == 1ull << val; }

        constexpr bool operator!=(const unsigned val) const { return flag != 1ull << val; }

        constexpr bool operator==(const DronePropLU & other) const { return other.flag == flag; }

        constexpr bool operator!=(const DronePropLU & other) const { return other.flag != flag; }

        constexpr operator bool() const { return flag != 0ull; }
    };

    static_assert(getDroneDataType("DEFAULT_VALUE_MAX") < MEM_MAX_ELEMENT_TYPE_COUNT,
                  "Field left for type information in DroneDataHandle is too small! Adjust bit "
                  "size of MAX_ELEMENT_BIT_COUNT to compensate for the number of property types.");


} // namespace hive