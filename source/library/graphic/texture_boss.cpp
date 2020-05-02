#include "./texture_boss.hpp"

namespace hive
{
    using namespace math;

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

            // data.texture.setData(ubyte_vec.data(), GL_RGBA, GL_UNSIGNED_BYTE);

            data.IN_VIDEO_RAM = true;

            data.ram_data.clear();
            data.IN_MAIN_RAM = false;
        }

        // data.texture.bind(unit);
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

                } else { /*
                     tex_data.texture =
                         Texture(GL_TEXTURE_2D, GL_RGBA, 0, png_width, png_height, bitdepth);


                     // tex_data.texture.setMagFilter(GL_NEAREST);
                     // tex_data.texture.setMinFilter(GL_NEAREST);
                     // tex_data.texture.setTexWrapS(GL_REPEAT);
                     // tex_data.texture.setTexWrapT(GL_REPEAT);
                     tex_data.height    = png_height;
                     tex_data.width     = png_width;
                     tex_data.bit_depth = bitdepth;
                     tex_data.ram_data  = texture_data;

                     tex_id = textures.size();

                     textures.push_back(tex_data);
                     */
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
