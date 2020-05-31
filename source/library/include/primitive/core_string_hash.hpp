#pragma once

#include <climits>
#include <iostream>
#include <string>
#include <sys/types.h>

namespace hive
{
    typedef unsigned long long hive_string_hash_64_ull;
    typedef hive_string_hash_64_ull str_ull;
    /**
     * If this bit is set then the StringHash64 instance contains a hash of a regular string.
     * If it is not set, then the hash is a literal 2^63 natural number.
     */
    constexpr static const str_ull STRING_HASH_STRING_FLAG = (str_ull)1 << 63;

    /**
     * If this bit is set then the StringHash64 instance store a compressed string
     * up to 10 characters long. The set of characters that it stores are described in
     * this regular expression:
     *      [0-9a-zA-Z_-\ ]
     *
     * If this bit is not set then the StringHash64 instance either contains a
     * long string hash if STRING_HASH_STRING_FLAG is set, or a 2^63 natural number.
     */
    constexpr static const str_ull STRING_HASH_SMALL_STRING_FLAG     = (str_ull)1 << 62;
    constexpr static const str_ull STRING_HASH_LONG_STRING_MASK      = ~((str_ull)1 << 62);
    constexpr static const str_ull STRING_HASH_SMALL_STRING_SENTINEL = (str_ull)0xF;
    constexpr static const unsigned STRING_HASH_MAX_INLINE_SIZE      = 10;
    constexpr static const u_short INV                               = 0xFF00;
    constexpr static const u_short SPC                               = 0x20;
    constexpr static const u_short HYP                               = (0x2D - 1);
    constexpr static const u_short NUM                               = (0x30 - 2);
    constexpr static const u_short UPC                               = (0x41 - 12);
    constexpr static const u_short UND                               = (0x5F - 1);
    constexpr static const u_short LWC                               = (0x61 - 38);
    constexpr static const u_short ASCII_LU[128]                     = {
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
        /**
         * Fowler-Noll-Vo Hash Function
         */
        constexpr static str_ull FNVHash(const char * string, const unsigned size)
        {
            const str_ull prime = 0x100000001b3;
            const str_ull basis = 0xcbf29ce484222325;
            str_ull hash        = prime;

            for (int i = 0; i < size; i++) hash = (hash * basis) ^ string[i];

            return hash;
        };

        // These two functions may need to be adjusted to handle threaded access.
        static std::string getLongString(str_ull);

        static void setLongString(str_ull, std::string);

        constexpr bool construct(const char * string, const unsigned size)
        {


            str_ull num = 0;
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
            u_short lu = 0;

            if (size <= STRING_HASH_MAX_INLINE_SIZE) {

                unsigned char invalid = 0;
                unsigned char type    = 0;
                int i                 = 0;

                for (; i < size; i++) {

                    const str_ull c = string[i];

                    lu = ASCII_LU[c & 0xFF];

                    const char lu_t = (lu >> 8);

                    type |= lu;

                    invalid |= ((c & 0xA) | lu_t);

                    num |= ((c - (lu & 0xFF)) << ((i << 1) + (i << 2)));
                }

                if (invalid < 128) {
                    if (NUM != type) {
                        val = num | STRING_HASH_STRING_FLAG | STRING_HASH_SMALL_STRING_FLAG;
                        return true;
                    };
                };
            }

            num = 0;

            str_ull int_offset = 1;

            for (int i = 0; i < size; i++) {

                const str_ull c = string[size - 1 - i];

                lu = ASCII_LU[c & 0xFF];

                if (lu != NUM) break;

                num += (c - 0x30) * int_offset;

                int_offset *= 10;
            }

            if (num < ULLONG_MAX && lu == NUM) {
                val = num;
                return true;
            }

            return false;
        }
        /**
         * @brief Hashes a string with unknown characters or that is longer than 10 characters.
         * Stores the string an a lookup table and sets val to hash value of the string
         * with a bitwise OR with @name STRING_HASH_STRING_FLAG and AND with @name
         * STRING_HASH_LONG_STRING_MASK.
         *
         */
        str_ull createLongStringHashAndStore(const char * string, const unsigned length)
        {
            str_ull hash = createLongStringHash(string, length);

            setLongString(hash, std::string(string, length));

            return hash;
        }

        /**
         * @brief Hashes a string with unknown characters or that is longer than 10 characters.
         * Stores the string an a lookup table and sets val to hash value of the string
         * with a bitwise OR with @name STRING_HASH_STRING_FLAG and AND with @name
         * STRING_HASH_LONG_STRING_MASK.
         *
         */
        constexpr str_ull createLongStringHash(const char * string, const unsigned length)
        {
            val =
                (FNVHash(string, length) | STRING_HASH_STRING_FLAG) & STRING_HASH_LONG_STRING_MASK;

            return val;
        }

        // Assumes check for short string storage performed prior to calling.
        // Extracts and recompiles char string buffer from [val].
        // [string] - char buffer with enough storage to store up to 8 characters
        // [number] - store for the length of string
        void getShortString(char * string, unsigned & number) const;

        str_ull val;

        static const std::size_t NULL_TERMINATED_SIZE_LIMIT = 128;

      public:
        /**
         * Constructs the hash of the string
         */
        StringHash64(const std::string string) : val(0)
        {
            if (!construct(string.data(), string.size()))
                createLongStringHashAndStore(string.data(), string.size());
        }

        StringHash64(const char * string, const unsigned size) : val(0)
        {
            if (!construct(string, size)) createLongStringHashAndStore(string, size);
        }

        constexpr StringHash64(const str_ull num) : val(num & ~STRING_HASH_STRING_FLAG) {}

        constexpr StringHash64(const unsigned num) : val(num) {}

        constexpr StringHash64(const int num) : val((unsigned)num) {}

        // constexpr StringHash64(const unsigned char num) : val(num) {}

        constexpr StringHash64(const char * string) : val(0)
        {
            if (string == nullptr) return;

            // check for null. Limit to 64 characters.
            for (int i = 0; i < NULL_TERMINATED_SIZE_LIMIT; i++) {
                if (string[i] == 0) {
                    if (i > 0)
                        if (!construct(string, i)) createLongStringHash(string, i);
                    return;
                }
            }
        }

        constexpr StringHash64(const StringHash64 & sh) : val(sh.val) {}

        constexpr bool operator==(const str_ull & v) const { return val == v; }

        constexpr bool operator==(const StringHash64 & v) const { return val == v.val; }

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

        constexpr operator str_ull() const { return val; }

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

    const unsigned long long test_string = StringHash64("a11d22222222222222222222a22222222am");

    // Static tests
    static_assert(StringHash64("a11d22222222222222222222a22222222am") !=
                      StringHash64("a11d22222222222222222222a22222222as"),
                  "Expected Expression to match");
    static_assert(StringHash64("1PropGPUProgram1") == StringHash64("1PropGPUProgram1"),
                  "Expected Expression to match");
    static_assert(StringHash64(10) == StringHash64(10), "Expected Expression to match");
    static_assert(StringHash64(10) == 10ull, "Expected Expression to match");
    static_assert(StringHash64("10") == StringHash64(10), "Expected Expression to match");

} // namespace hive