#pragma once

/**
 * Some good ole type defs for convienence
 */
namespace hive
{
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
} // namespace hive
