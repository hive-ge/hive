#include "include/hive.h"
#include <map>
#include <sys/types.h>

namespace hive
{
    static std::map<u_ull, std::string> long_string_table;

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

} // namespace hive