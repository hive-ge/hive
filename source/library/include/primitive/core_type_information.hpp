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

#define DroneDataByteSize 16

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
            return MEM_MAX_ELEMENT_TYPE_COUNT - 1;
            // DroneDataHandle for type information
            // Complex Systems
            SetPropIndexLookup(mesh);        // 1
            SetPropIndexLookup(image);       // 2
            SetPropIndexLookup(grid);        // 3
            SetPropIndexLookup(script);      // 4
            SetPropIndexLookup(material);    // 5
            SetPropIndexLookup(gpu_program); // 6
            SetPropIndexLookup(parent);      // 7
            SetPropIndexLookup(animation);   // 8
            SetPropIndexLookup(audio);       // 9

            // Numerics
            SetPropIndexLookup(mat44d);  // 10
            SetPropIndexLookup(mat44);   // 11
            SetPropIndexLookup(mat33d);  // 12
            SetPropIndexLookup(mat33);   // 13
            SetPropIndexLookup(quat);    // 14
            SetPropIndexLookup(quatd);   // 15
            SetPropIndexLookup(vec3d);   // 16
            SetPropIndexLookup(vec3);    // 17
            SetPropIndexLookup(vec2d);   // 18
            SetPropIndexLookup(vec2);    // 19
            SetPropIndexLookup(float32); // 20
            SetPropIndexLookup(float64); // 21
            SetPropIndexLookup(int32);   // 22
            SetPropIndexLookup(uint32);  // 23
            SetPropIndexLookup(int64);   // 24
            SetPropIndexLookup(uint64);  // 25

            // Attributes
            SetPropIndexLookup(render);      // 26
            SetPropIndexLookup(renderlayer); // 27
            SetPropIndexLookup(tag);         // 28

            return i;
        }

        return 0;
    }

#define setDroneLUStaticBITFlag(prop) prop = 1u << getDroneDataType(#prop)
#define setDroneTypeEnumEntry(prop) prop = getDroneDataType(#prop)

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
            setDroneLUStaticBITFlag(renderlayer),
            setDroneLUStaticBITFlag(tag)
        };


        enum class PropType : unsigned char {
            setDroneTypeEnumEntry(material),
            setDroneTypeEnumEntry(mesh),
            setDroneTypeEnumEntry(grid),
            setDroneTypeEnumEntry(image),
            setDroneTypeEnumEntry(script),
            setDroneTypeEnumEntry(gpu_program),
            setDroneTypeEnumEntry(parent),
            setDroneTypeEnumEntry(animation),
            setDroneTypeEnumEntry(audio),
            setDroneTypeEnumEntry(mat44d),
            setDroneTypeEnumEntry(mat44),
            setDroneTypeEnumEntry(mat33d),
            setDroneTypeEnumEntry(mat33),
            setDroneTypeEnumEntry(quat),
            setDroneTypeEnumEntry(quatd),
            setDroneTypeEnumEntry(vec3d),
            setDroneTypeEnumEntry(vec3),
            setDroneTypeEnumEntry(vec2d),
            setDroneTypeEnumEntry(vec2),
            setDroneTypeEnumEntry(float32),
            setDroneTypeEnumEntry(float64),
            setDroneTypeEnumEntry(int32),
            setDroneTypeEnumEntry(uint32),
            setDroneTypeEnumEntry(int64),
            setDroneTypeEnumEntry(uint64),
            setDroneTypeEnumEntry(render),
            setDroneTypeEnumEntry(renderlayer),
            setDroneTypeEnumEntry(tag)
        };

        union {
            hive_ull flag;
            PropType DroneType;
        };

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
            flag |= 1ull << val;
            return *this;
        }

        constexpr DronePropLU & operator-=(const unsigned val)
        {
            flag &= ~(1ull << val);
            return *this;
        }


        constexpr DronePropLU & operator=(const unsigned val)
        {
            flag |= (1ull << val);
            return *this;
        }

        constexpr bool operator==(const hive_ull val) const { return (flag & val) == val; }

        constexpr bool operator!=(const hive_ull val) const { return (flag & val) != val; }

        constexpr bool operator==(const unsigned short val) const
        {
            return (flag & (1ull << val)) > 0;
        }

        constexpr bool operator!=(const unsigned short val) const
        {
            return (flag & (1ull << val)) == 0;
        }

        constexpr bool operator==(const DronePropLU & other) const
        {
            return (flag & other.flag) == other.flag;
        }

        constexpr bool operator!=(const DronePropLU & other) const { return !(*this == other); }

        constexpr operator bool() const { return flag != 0ull; }

        constexpr operator hive_ull() const { return flag; }
    };

    static_assert(getDroneDataType("DEFAULT_VALUE_MAX") < MEM_MAX_ELEMENT_TYPE_COUNT,
                  "Field left for type information in DroneDataHandle is too small! Adjust bit "
                  "size of MAX_ELEMENT_BIT_COUNT to compensate for the number of property types.");


} // namespace hive