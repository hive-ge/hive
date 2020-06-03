#pragma once

#include "core_type_information.hpp"
#include <iostream>

namespace hive
{

    typedef unsigned drone_flag_primitive;


    constexpr unsigned SetFlagOffset(const drone_flag_primitive offset) { return 1 << offset; };

    template <drone_flag_primitive offset = 0> struct DroneFlagTemplate {

        const static drone_flag_primitive value = SetFlagOffset(offset);

        drone_flag_primitive flag = value;

        constexpr operator unsigned() const { return flag; }

        constexpr DroneFlagTemplate(drone_flag_primitive i) : flag(i) {}

        constexpr DroneFlagTemplate() {}

        template <drone_flag_primitive i>
        friend constexpr DroneFlagTemplate<offset> operator+(const DroneFlagTemplate<offset> a,
                                                             const DroneFlagTemplate<i> b)
        {
            return DroneFlagTemplate<offset>(a.flag | b.flag);
        }

        template <drone_flag_primitive i>
        constexpr DroneFlagTemplate & operator+=(const DroneFlagTemplate<i> & other)
        {
            flag |= other.flag;
            return *this;
        }

        template <drone_flag_primitive i>
        constexpr DroneFlagTemplate & operator-=(const DroneFlagTemplate<i> & other)
        {
            flag &= ~other.flag;
            return *this;
        }

        template <drone_flag_primitive i>
        constexpr DroneFlagTemplate & operator=(const DroneFlagTemplate<i> & other)
        {
            flag |= other.flag;
            return *this;
        }

        template <drone_flag_primitive i>
        constexpr bool operator==(const DroneFlagTemplate<i> & other) const
        {
            return other.flag & flag;
        }

        template <drone_flag_primitive i>
        constexpr bool operator!=(const DroneFlagTemplate<i> & other) const
        {
            return !(other.flag & flag);
        }

        template <drone_flag_primitive i>
        constexpr DroneFlagTemplate<0> operator|(const DroneFlagTemplate<i> & other) const
        {
            return DroneFlagTemplate<0>(flag | other.flag);
        }
    };

    typedef DroneFlagTemplate<0> flag_A;
    constexpr flag_A FLAG_A;
    typedef DroneFlagTemplate<1> flag_B;
    constexpr flag_B FLAG_B;
    typedef DroneFlagTemplate<2> flag_C;
    constexpr flag_C FLAG_C;
    typedef DroneFlagTemplate<3> flag_D;
    constexpr flag_D FLAG_D;
    typedef DroneFlagTemplate<4> flag_E;
    constexpr flag_E FLAG_E;
    typedef DroneFlagTemplate<5> flag_F;
    constexpr flag_F FLAG_F;
    typedef DroneFlagTemplate<6> flag_G;
    constexpr flag_G FLAG_G;
    typedef DroneFlagTemplate<7> flag_H;
    constexpr flag_H FLAG_H;
    typedef DroneFlagTemplate<8> flag_I;
    constexpr flag_I FLAG_I;
    typedef DroneFlagTemplate<9> flag_J;
    constexpr flag_J FLAG_J;
    typedef DroneFlagTemplate<10> flag_K;
    constexpr flag_K FLAG_K;
    typedef DroneFlagTemplate<11> flag_L;
    constexpr flag_L FLAG_L;
    typedef DroneFlagTemplate<12> flag_M;
    constexpr flag_M FLAG_M;
    typedef DroneFlagTemplate<13> flag_N;
    constexpr flag_N FLAG_N;
    typedef DroneFlagTemplate<14> flag_O;
    constexpr flag_O FLAG_O;
    typedef DroneFlagTemplate<15> flag_P;
    constexpr flag_P FLAG_P;
    typedef DroneFlagTemplate<16> flag_Q;
    constexpr flag_Q FLAG_Q;
    typedef DroneFlagTemplate<17> flag_R;
    constexpr flag_R FLAG_R;
    typedef DroneFlagTemplate<18> flag_S;
    constexpr flag_S FLAG_S;
    typedef DroneFlagTemplate<19> flag_T;
    constexpr flag_T FLAG_T;
    typedef DroneFlagTemplate<20> flag_U;
    constexpr flag_U FLAG_U;
    typedef DroneFlagTemplate<21> flag_V;
    constexpr flag_V FLAG_V;
    typedef DroneFlagTemplate<22> flag_W;
    constexpr flag_W FLAG_W;
    typedef DroneFlagTemplate<23> flag_X;
    constexpr flag_X FLAG_X;
    typedef DroneFlagTemplate<24> flag_Y;
    constexpr flag_Y FLAG_Y;
    typedef DroneFlagTemplate<25> flag_Z;
    constexpr flag_Z FLAG_Z;
    typedef DroneFlagTemplate<26> flag_0;
    constexpr flag_0 FLAG_0;

    constexpr unsigned d = FLAG_A + FLAG_E + FLAG_A;

    static_assert(FLAG_A != FLAG_Z, "FLAG_A should not equal FLAG_Z");
    static_assert(FLAG_A + FLAG_B == 3ull, "FLAG_A should not equal FLAG_Z");
    static_assert(FLAG_A + FLAG_C == 5ull, "FLAG_A should not equal FLAG_Z");
    static_assert(FLAG_A + FLAG_D == 9ull, "FLAG_A should not equal FLAG_Z");
    static_assert(FLAG_A + FLAG_E == 17ull, "FLAG_A should not equal FLAG_Z");
    static_assert(FLAG_A + FLAG_E + FLAG_A + FLAG_E == 17ull, "FLAG_A should not equal FLAG_Z");

}; // namespace hive
