#include "include/hive.h"
#include <map>
#include <sys/types.h>

namespace hive
{
    static std::map<u_ull, std::string> long_string_table;

    u_ull StringHash64::hashLongString(const char * string, const unsigned size)
    {
        u_ull seed = 0X15F23100121;
        u_ull hash = 0;

        for (int i = 0; i < size; i++) {
            hash = (hash * seed) + string[i];
        }

        return hash;
    };

    std::string StringHash64::getLongString(u_ull hash_key)
    {
        return long_string_table.at(hash_key);
    };

    void StringHash64::setLongString(u_ull hash_key, std::string str)
    {
        long_string_table.insert({hash_key, str});
    };

    void StringHash64::getShortString(char * string, unsigned & i) const
    {
        i = 0;

        for (; i < STRING_HASH_MAX_INLINE_SIZE; i++) {

            unsigned char d = (val >> (u_ull)((i << 1) + (i << 2))) & 0x3F;

            if (d == 0)
                return;
            else if (d == 0)
                d += SPC;
            else if (d == 1)
                d += HYP;
            else if (d < 12)
                d += NUM;
            else if (d < 38)
                d += UPC;
            else
                d += LWC;

            string[i] = d;
        }
    }

    void StringHash64::construct(const char * string, const unsigned size)
    {
        u_ull num = 0;
        /**
         * Compress ASCII strings into a 2^48 space
         * This will leave 16 bits for irregular strings
         * that contain non ascii characters or are larger than
         * 8 characters.
         *
         * This allows for a reduced 64 character set for ASCII strings
         * Which allows for uppercase and lowercase latin characters,
         * numbers, and punctuation.
         *
         * 32 ->
         * for longer string
         */
        if (size <= STRING_HASH_MAX_INLINE_SIZE) {

            unsigned char invalid = 0;
            unsigned char type    = 0;
            int i                 = 0;

            for (; i < size; i++) {
                const u_ull c    = string[i];
                const u_short lu = ASCII_LU[c & 0xFF];
                const char lu_t  = (lu >> 8);

                type |= invalid |= ((c & 0xA) | lu_t);

                num |= ((c - (lu & 0xFF)) << ((i << 1) + (i << 2)));
            }

            if (invalid >= 128) goto fallback;

            if (NUM == type) goto natural;

            val = num | STRING_HASH_STRING_FLAG |
                  STRING_HASH_SMALL_STRING_FLAG; // |
                                                 // (STRING_HASH_SMALL_STRING_SENTINEL << ((i << 1)
                                                 // + (i << 2)));

            return;
        }

    natural:
        num = 0;

        for (int i = 0; i < size; i++) {

            const unsigned char c = string[i];

            const short lu = ASCII_LU[c & 0xFF];

            if (lu != NUM) goto fallback;

            num += ((c - (lu & 0xFF)) << ((i << 3) + (i << 1)));
        }

        if (num < ULLONG_MAX) {
            val = num;
            return;
        }

    fallback:
        std::cout << " longs string" << std::endl;

        val =
            (hashLongString(string, size) | STRING_HASH_STRING_FLAG) & STRING_HASH_LONG_STRING_MASK;

        setLongString(val, std::string(string, size));

        return;
    }

} // namespace hive