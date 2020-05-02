#pragma once


#include <string>
#include <sys/types.h>
#include <vector>

#include "3rdparty/libspng/spng.h"

#include "gpu/gpu_primitive.hpp"

#include "primitive/boss.hpp"

#include "primitive/drone.hpp"

#include "primitive/math/general.hpp"

#include "primitive/typedef.hpp"

#include "resource/load.hpp"


namespace hive
{
    using namespace math;

    class TextureBoss;

    struct TextureID {
        unsigned id;
    };

    struct TextureProp : Prop {
        friend TextureBoss;

      public:
        static const ushort TYPE = TEXTURE_PROP_TYPE;

      public:
        TextureProp() : Prop(TYPE, sizeof(TextureProp)) {}

        unsigned type = 0;

        // As long as texture is referenced by on prop then the
        // texture will not evacuated from memory.
        ushort width  = 0;
        ushort height = 0;

        inline TextureID getID() { return id; }

      protected:
        TextureID id;
        TextureBoss * boss = nullptr;

      public:
        void useTexture(unsigned unit = 0);
    };

    struct TextureData {
        // hive::Texture texture;
        // hive::VRAMBuffer vram_data;
        std::vector<ubyte> ram_data;
        bool IN_VIDEO_RAM = false;
        bool IN_MAIN_RAM  = false;
        ubyte bit_depth   = 8;
        unsigned width    = 0;
        unsigned height   = 0;
        unsigned depth    = 0;
    };

    /**
     * Set of common depth properties for textures.
     */
    enum TextureDepth {
        /**
         * 3 Channels.
         * 8 Bits per channel.
         */
        R8_G8_B8 = 24,
        /**
         * 4 Channels.
         * 8 Bits per channel.
         */
        R8_G8_B8_A8 = 32,

        /**
         * 4 Channels.
         * 16 Bits per channel.
         */
        R16_G16_B16_A16 = 64
    };

    /**
     * Handles the management and rendering of sprites using instanced render passes.
     */
    class TextureBoss : public Boss
    {
        friend TextureProp;

      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_TEXTURE;

      private:
        /**
         * Number of sprites that are being rendered.
         */
        int active_texures = 0;

        std::vector<TextureData> textures;

      public:
        TextureBoss() : Boss(IDENTIFIER) {}
        virtual ~TextureBoss() {}

        /**
         * Creates and returns a texture prop
         * from a buffer resource. */
        TextureProp * createTexture(char * buffer, TextureDepth, unsigned width, unsigned height,
                                    unsigned depth);
        /**
         * Creates and returns a texture prop
         * from an image file.
         *
         * TODO run on seperate thread and return early.
         */
        TextureProp * createTexture(const std::string img_path);


      protected:
        virtual void update(float delta_t);
        virtual void setup();
        virtual void teardown();
        virtual int priority();
        /**
         * Signals that the texture is intended to be used and to
         * tranfer data to video memeory.
         */
        void useTexture(TextureID id, unsigned unit = 0);
    };
} // namespace hive
