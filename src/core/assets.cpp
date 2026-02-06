#include "assets.h"

Sprite getSprite(SpriteID spriteID)
{
    Sprite sprite = {};
    switch (spriteID)
    {
    case SPRITE_LOGO:
    {
        sprite.offset = {0, 0};
        sprite.size = {16, 16};
    }
    }
    return sprite;
}