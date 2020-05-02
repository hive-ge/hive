#pragma once

//#include "gpu/gpu_primitive.hpp"
#include "graphic/texture_boss.hpp"
#include "primitive/boss.hpp"
#include "primitive/drone.hpp"
#include "primitive/math/math.hpp"
#include <vector>

namespace hive
{
    using namespace math;

    struct Sprite {

        /**
         * Index id of the sprites texture.
         */
        int tex_id;

        /**
         * 2D cooridinates of texture space
         * in unormalized ixel units.
         */
        vec2i pix_pos;

        /**
         * 2D width and height of the
         * texture view window.
         *
         * if 0,0. then the whole texture
         * bounds are used.
         */
        vec2i pix_size;

        /**
         * 3D world position of the sprite in
         * world units.
         */
        vec3 pos;

        /**
         * Local Euler rotation of sprite.
         */
        vec3 rot;

        /**
         * Local scale of sprite.
         */
        vec3 scale;

        TextureProp texture;
    };


    //::HIVE DRONE_PROP
    struct SpriteProp : Prop {
      public:
        static const ushort TYPE = SPRITE_PROP_TYPE;

      public:
        /**
         * Database identifier of the sprite
         */
        unsigned sprite_id;

      public:
        SpriteProp() : Prop(TYPE, sizeof(SpriteProp)){};

        static SpriteProp * construct() { return new SpriteProp(); }

        // Upload new sprite data.

        // Set sprite bounds.

        // Set sprite offset.
    };

    /**
     * Handles the management and rendering of sprites using instanced render passes.
     */
    class SpriteBoss : public Boss
    {
      public:
        static const unsigned IDENTIFIER = BOSS_IDENTIFIER_SPRITE;


      private:
        TextureBoss * tex_boss;

        std::vector<Sprite> sprites;

        /**
         * Number of sprites that are being rendered.
         */
        int active_sprites = 0;

      public:
        SpriteBoss() : Boss(IDENTIFIER)
        {

            TextureBoss * temp = static_cast<TextureBoss *>(getBoss(TextureBoss::IDENTIFIER));

            if (!temp) throw 0;

            tex_boss = temp;
        }

        SpriteProp * loadSprite(const std::string);

        // Called by BigBadBoss instance.
        virtual void update(float step){};

        // Setup of data
        virtual void setup(){};

        // Called before exit
        virtual void teardown(){};

        // Called periodicolly to update priority values
        virtual int priority() { return 10000; };
    };
} // namespace hive