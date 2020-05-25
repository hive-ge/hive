#include <vector>

#include "include/hive.h"

namespace hive
{
    /*
        Optional Implmentation, useful for debugging
    */
    bool ImageProp::loadFromFile(const std::string file_name, const bool STREAM_TO_VRAM)
    {
        return false;
    };

    /**
     */
    bool ImageProp::loadFromRawBinaryData(const char * buffer, const unsigned size,
                                          const unsigned width, const unsigned height,
                                          const bool STREAM_TO_VRAM)
    {
        return false;
    };

    bool ImageProp::uploadToVRAM() { return false; };

    bool ImageProp::downloadFromVRAM() { return false; };

    bool ImageProp::clearFromRAM() { return false; };

    const char * ImageProp::getRawData() { return nullptr; };

    bool ImageProp::IS_IN_VRAM() { return false; };

    bool ImageProp::IS_IN_RAM() { return false; };

    bool ImageProp::setRenderTarget() { return false; };

    bool ImageProp::clearRenderTarget() { return false; };
} // namespace hive