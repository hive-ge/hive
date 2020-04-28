#pragma once

#include <string>

#include "primitive/typedef.h"

namespace hive
{ 
    namespace gpu
    {
        /**
         * Class Buffer
         * Represents a block of VRAM
         */
        class Buffer
        {
          private:
          public:
            /**
             * Returns the current size of the buffer.
             *
             * Automatically expand based on requirements of input data.
             */
            unsigned size();

            /**
             * Copies data to a managed store and prepares
             * the data to be loaded into the buffer.
             *
             * Returns immediatly.
             */
            HIVE_RESULT copyAndLoad(char * buffer, size_t buffer_size, unsigned offset);

            HIVE_RESULT load(char * buffer, size_t buffer_size, unsigned offset);

            /**
             * Unload stored data from GPU.
             */
            HIVE_RESULT unload();
        };

        /**
         * Class Texture
         *
         * Represents a drawing surface residing in GPU memory
         * which can be used as both a read and write region
         * for rendering operations.
         *
         * Any specific graphics API specific implementation
         * should inherit from this class and implement the
         * operations described here.
         */

        class Image
        {
          private:
            unsigned width  = 0;
            unsigned height = 0;
            /**
             * Number of color components
             * per image unit.
             * 1 component can repressent a Grayscale image
             * 3 components can represent RGB
             * 4 components can represent RGBA
             */
            unsigned component_count = 3; // Defaults to RGB

            /**
             * Bytes per image component
             */
            unsigned bytedepth = 1;

          public:
            /**
             * Configure the image to be used as a read target for the next
             * render pass within the specified texture slot index.
             */
            HIVE_RESULT setReadSlot(unsigned texture_slot_index);

            /**
             * Configure the image to be used as a write target for the
             * next render pass at the specified render buffer index
             * position
             */
            HIVE_RESULT setWriteSlot(unsigned render_buffer_slot_index);

            HIVE_RESULT loadPNGImage(std::string image_file_path);
            HIVE_RESULT loadPNGImage(char * buffer, size_t buffer_size);

            HIVE_RESULT loadJPEGImage(std::string image_file_path);
            HIVE_RESULT loadJPEGImage(char * buffer, size_t buffer_size);

            /**
             * Create an image from a buffer of RGBA or RGB elements.
             *
             * Buffer size should be (width * height * component_count)
             */
            bool loadRAWImage(char * buffer, unsigned width, unsigned height,
                              unsigned component_count);

            bool swizzle();
        };
    } // namespace gpu
} // namespace hive