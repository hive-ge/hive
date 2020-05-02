#pragma once

#include <vector>

#include "primitive/math/math.hpp"

#include "primitive/prop.hpp"

namespace hive
{
    using namespace hive::math;

    struct ImageData {
        void * data = nullptr;
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
        bool ON_GPU : 1, HAS_UV : 1, HAS_VERT : 1, HAS_INDICES : 1;
    };

    //::HIVE DRONE_PROP
    struct ImageProp : Prop {

        static ImageProp * construct() { return new ImageProp(); }

        ImageProp() : Prop(IMAGE_PROP_TYPE, sizeof(ImageProp))
        {
            setData<ImageData>(new ImageData);
        }

        ~ImageProp()
        {
            ImageData * data = getData<ImageData>();
            delete data;
        }
        // void removeVertex();
    };
} // namespace hive