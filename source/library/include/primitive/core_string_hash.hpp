
#pragma once

#include <climits>
#include <iostream>
#include <string>
#include <sys/types.h>

namespace hive
{

    typedef unsigned long long u_ull;

    static const u_ull STRING_HASH_STRING_FLAG           = (u_ull)1 << 63;
    static const u_ull STRING_HASH_SMALL_STRING_FLAG     = (u_ull)1 << 62;
    static const u_ull STRING_HASH_LONG_STRING_MASK      = ~((u_ull)1 << 62);
    static const u_ull STRING_HASH_SMALL_STRING_SENTINEL = (u_ull)0xF;
    static const unsigned STRING_HASH_MAX_INLINE_SIZE    = 10;
    static const u_short INV                             = 0xFF00;
    static const u_short SPC                             = 0x20;
    static const u_short HYP                             = (0x2D - 1);
    static const u_short NUM                             = (0x48 - 2);
    static const u_short UPC                             = (0x41 - 12);
    static const u_short UND                             = (0x5F - 1);
    static const u_short LWC                             = (0x61 - 38);
    static const u_short ASCII_LU[128]                   = {
        INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV,
        INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV,
        SPC, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, INV, HYP, INV, INV,
        NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, INV, INV, INV, INV, INV, INV,
        INV, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC,
        UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, UPC, INV, INV, INV, INV, UND,
        INV, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC,
        LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, LWC, INV, INV, INV, INV, INV};


    struct StringHash64 {


      private:
        static u_ull hashLongString(const char * string, const unsigned size);

        // These two functions may need to be adjusted to handle threaded access.
        static std::string getLongString(u_ull);
        static void setLongString(u_ull, std::string);

        void construct(const char * string, const unsigned size);

        // Assumes check for short string storage performed prior to calling.
        // Extracts and recompiles char string buffer from [val].
        // [string] - char buffer with enough storage to store up to 8 characters
        // [number] - store for the length of string
        void getShortString(char * string, unsigned & number) const;

        u_ull val;

        static const std::size_t NULL_TERMINATED_SIZE_LIMIT = 16;

      public:
        StringHash64(const char * string, const unsigned size) : val(0) { construct(string, size); }

        StringHash64(const std::string string) : val(0) { construct(string.data(), string.size()); }

        StringHash64(const char * string) : val(0)
        {
            // check for null. Limit to 64 characters.
            for (int i = 0; i < NULL_TERMINATED_SIZE_LIMIT; i++) {
                if (string[i] == 0) {
                    construct(string, i);
                    return;
                }
            }
            std::cout << "test" << std::endl;
        }

        // explicit StringHash64(const u_ull v) : val(0)
        // {
        //     if (v < STRING_HASH_MAX_INLINE_SIZE) val = 0;
        // }

        StringHash64(const StringHash64 & sh) : val(sh.val) {}

        bool operator==(const u_ull & v) const { return val == v; }
        bool operator==(const StringHash64 & v) const { return val == v.val; }

        friend std::ostream & operator<<(std::ostream & stream, const StringHash64 & sh)
        {
            if (sh.val & STRING_HASH_STRING_FLAG) {
                if (sh.val & STRING_HASH_SMALL_STRING_FLAG) {
                    char string[8];
                    unsigned length;
                    sh.getShortString(string, length);
                    stream << std::string(string, length);
                } else {
                    stream << getLongString(sh.val);
                }
            } else {
                stream << sh.val;
            }

            return stream;
        }

        explicit operator u_ull() const { return val; }

        explicit operator std::string() const
        {
            std::string str = "";

            if (val & STRING_HASH_STRING_FLAG) {
                if (val & STRING_HASH_SMALL_STRING_FLAG) {
                    char string[8];
                    unsigned length;
                    getShortString(string, length);
                    str = std::string(string, length);
                } else {
                    str += getLongString(val);
                }
            } else {
                str += val;
            }

            return str;
        }
    };

    const StringHash64 NullStringHash("");
} // namespace hive