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

    bool ImageProp::downloadFromVRAM() { return false; };

    bool ImageProp::clearFromRAM() { return false; };

    const char * ImageProp::getRawData() { return nullptr; };

    bool ImageProp::IS_IN_VRAM() { return false; };

    bool ImageProp::IS_IN_RAM() { return false; };

    bool ImageProp::setRenderTarget() { return false; };

    bool ImageProp::clearRenderTarget() { return false; };

    void ImageProp::setPixel(unsigned x, unsigned y, unsigned r, unsigned g, unsigned b, unsigned a)
    {
        const unsigned index = (x + y * width) * 3;

        if (index > width * height * depth * 3) return;

        if (index > 0) {
            data[index]     = r;
            data[index + 1] = g;
            data[index + 2] = b;
        }
    }

    void ImageProp::setSize(unsigned w, unsigned h, unsigned d)
    {
        width  = w;
        height = h;
        depth  = d;
        data.resize(w * h * d * 3);
    }
} // namespace hive