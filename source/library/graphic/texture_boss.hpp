#pragma once


#include <string>
#include <sys/types.h>
#include <vector>

#include "3rdparty/libspng/spng.h"

#include "gpu/gpu_primitive.hpp"

#include "primitive/boss.hpp"

#include "primitive/drone/drone.h"

#include "primitive/math/general.h"

#include "primitive/typedef.h"

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
        hive::Texture texture;
        hive::VRAMBuffer vram_data;
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


    //####################################### Definition

    void TextureBoss::update(float delta_t) {}

    int TextureBoss::priority() { return 0; }

    void TextureBoss::setup(){};

    void TextureBoss::teardown(){};

    void TextureBoss::useTexture(TextureID id, unsigned unit)
    {
        TextureData & data = textures[id.id];

        if (!data.IN_VIDEO_RAM) {
            auto & ubyte_vec = data.ram_data;

            data.texture.setData(ubyte_vec.data(), GL_RGBA, GL_UNSIGNED_BYTE);

            data.IN_VIDEO_RAM = true;

            data.ram_data.clear();
            data.IN_MAIN_RAM = false;
        }

        data.texture.bind(unit);
        // data.texture.use(data.vram_data);
    }

    void TextureProp::useTexture(unsigned unit) { boss->useTexture(id, unit); }

    TextureProp * TextureBoss::createTexture(char * buffer, TextureDepth, unsigned width,
                                             unsigned height, unsigned depth)
    {
        // Create new buffer.

        // Optionally check to see if there is already a buffer holding this data.

        return nullptr;
    };


    TextureProp * TextureBoss::createTexture(const std::string img_path)
    {
        // Create new buffer.
        std::vector<ubyte> data = loadCharFile(img_path);

        int tex_id = -1;

        TextureData tex_data;
        TextureProp * prop = new TextureProp();

        // Test for PNG data
        {
            unsigned png_width  = 0;
            unsigned png_height = 0;
            unsigned error      = 0;

            spng_ctx * ctx = spng_ctx_new(0);

            /* Set an input buffer */

            if (ctx != NULL) {
                error = spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE);

                error = spng_set_png_buffer(ctx, data.data(), data.size());

                struct spng_ihdr ihdr;
                error = spng_get_ihdr(ctx, &ihdr);

                png_width  = ihdr.width;
                png_height = ihdr.height;

                size_t out_size = 0;
                // spng_get_image_limits(ctx, &png_width, &png_height);

                /* Calculate output image size */
                spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &out_size);


                unsigned bitdepth = 8;

                std::vector<ubyte> texture_data(png_width * png_height * 4);

                auto char_buffer = (texture_data.data());

                /* Get an 8-bit RGBA image regardless of PNG format */
                error = spng_decode_image(ctx, char_buffer, out_size, SPNG_FMT_RGBA8, 0);

                if (error) {

                } else {
                    tex_data.texture =
                        Texture(GL_TEXTURE_2D, GL_RGBA, 0, png_width, png_height, bitdepth);


                    tex_data.texture.setMagFilter(GL_LINEAR);
                    tex_data.texture.setMinFilter(GL_LINEAR);
                    tex_data.texture.setTexWrapS(GL_REPEAT);
                    tex_data.texture.setTexWrapT(GL_REPEAT);
                    tex_data.height    = png_height;
                    tex_data.width     = png_width;
                    tex_data.bit_depth = bitdepth;
                    tex_data.ram_data  = texture_data;

                    tex_id = textures.size();

                    textures.push_back(tex_data);
                }
            }

            /* Free context memory */
            spng_ctx_free(ctx);
        }

        if (tex_id >= 0) {
            prop->width  = tex_data.width;
            prop->height = tex_data.height;
            prop->id.id  = tex_id;
            prop->boss   = this;
        }

        // Optionally check to see if there is already a buffer holding this data.
        return prop;
    };


} // namespace hive
