#include "./sprite_boss.hpp"

namespace hive
{
    using namespace math;

    SpriteProp * SpriteBoss::loadSprite(const std::string texture_image)
    {

        TextureProp * texture;

        texture = tex_boss->createTexture(texture_image);

        SpriteProp & sprite_prop = (*new SpriteProp);

        // Next object in this chain is texture
        sprite_prop.next = texture;

        Sprite sprite;

        sprite_prop.sprite_id = sprites.size();

        sprites.push_back(sprite);

        return &sprite_prop;
    }
} // namespace hive