#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_memory_pool.hpp"
#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;

    typedef unsigned char u_char;

    struct ImageData {
        std::vector<unsigned char> data;
        // Texture location on gpu side.
        unsigned gpu_handle   = 0;
        unsigned short width  = 0;
        unsigned short height = 0;
        unsigned short depth  = 0;
        // Number of color channels per image. R (1), RGB (3), RGBA (4), ...
        unsigned char channels = 8;
        // Number of bits per color channel. Defaults to 8 (1 byte).
        unsigned char bit_size                          = 8;
        enum CompressionType : char { RAW } compression = CompressionType::RAW;
        bool IN_VRAM : 1, IN_RAM;
    };

    typedef DataPool::DataField<ImageData> ImageDataField;

    /**::HIVE DRONE_PROP::*/
    struct ImageProp : Prop {

        CONSTRUCT_PROP(ImageProp, image);

        /**
         * Changes this to a dynamic pointer that can point to:
         * GridProp * buffer
         */
        ImageDataField::PTR data = nullptr;

        ImageProp() { data = general_data_pool.allocate<ImageData>(); }

        ~ImageProp() {}

        /*
            Optional Implementation, useful for debugging
        */
        bool loadFromFile(const std::string file_name, const bool STREAM_TO_VRAM = false);

        /**
         */
        bool loadFromRawBinaryData(const char * buffer, const unsigned size, const unsigned width,
                                   const unsigned height, const bool STREAM_TO_VRAM = false);

        void setPixel(unsigned x = 0, unsigned y = 0, unsigned r = 0, unsigned g = 0,
                      unsigned b = 0, unsigned a = 0);

        bool uploadToVRAM();

        bool downloadFromVRAM();

        bool clearFromRAM();

        const char * getRawData();

        bool IS_IN_VRAM();

        bool IS_IN_RAM();

        bool setRenderTarget();

        bool clearRenderTarget();

        void setSize(unsigned width = 1, unsigned height = 1, unsigned depth = 1);

        void instance(ImageProp::Ref other_image)
        {
            if (other_image == ref) return;

            if (data) {
                // delete data;
            }

            data = other_image->data;
        }

        template <class T> T getTextureHandle();
    };

    REGISTER_PROP(ImageProp);
} // namespace hive