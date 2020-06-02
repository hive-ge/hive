#pragma once

#include <vector>

#include "include/primitive/core_math.hpp"
#include "include/primitive/core_prop.hpp"

namespace hive
{
    using namespace hive::math;


    struct GridProp : Prop {

        CONSTRUCT_PROP(GridProp, grid);

        ~GridProp() {}

        /*
            Optional Implmentation, useful for debugging
        */
        bool loadFromFile(const std::string file_name, const bool STREAM_TO_VRAM = false);

        /**
         */
        bool loadFromRawBinaryData(const char * buffer, const unsigned size, const unsigned width,
                                   const unsigned height, const bool STREAM_TO_VRAM = false);

        bool uploadToVRAM();

        bool downloadFromVRAM();

        bool clearFromRAM();

        const char * getRawData();

        bool IS_IN_VRAM();

        bool IS_IN_RAM();

        bool setRenderTarget();

        bool clearRenderTarget();

        template <class T> T getTextureHandle();
    };

    REGISTER_PROP(GridProp);
} // namespace hive