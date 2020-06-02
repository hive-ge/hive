#pragma once

#include <iostream>

namespace hive
{

    typedef unsigned drone_flag_primitive;


    constexpr unsigned SetFlagOffset(const drone_flag_primitive offset)
    {
        return offset > 0 ? 1 << (offset - 1) : 0;
    };

    template <drone_flag_primitive offset = 0> struct DroneFlagTemplate {

        const static drone_flag_primitive value = SetFlagOffset(offset);

        drone_flag_primitive flag = SetFlagOffset(offset);

        constexpr DroneFlagTemplate(drone_flag_primitive i) : flag(i) {}

        DroneFlagTemplate() {}

        template <drone_flag_primitive i>
        DroneFlagTemplate & operator+=(const DroneFlagTemplate<i> & other)
        {
            flag |= other.flag;
            return *this;
        }

        template <drone_flag_primitive i>
        DroneFlagTemplate & operator-=(const DroneFlagTemplate<i> & other)
        {
            flag &= ~other.flag;
            return *this;
        }

        template <drone_flag_primitive i>
        DroneFlagTemplate & operator=(const DroneFlagTemplate<i> & other)
        {
            flag |= other.flag;
            return *this;
        }

        template <drone_flag_primitive i> bool operator==(const DroneFlagTemplate<i> & other) const
        {
            return other.flag & flag;
        }

        template <drone_flag_primitive i> bool operator!=(const DroneFlagTemplate<i> & other) const
        {
            return !(other.flag & flag);
        }

        template <drone_flag_primitive i>
        constexpr DroneFlagTemplate<0> operator|(const DroneFlagTemplate<i> & other) const
        {
            return DroneFlagTemplate<0>(flag | other.flag);
        }
    };
}; // namespace hive
