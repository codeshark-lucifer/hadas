#include "assets.h"

Sprite getSprite(SpriteID spriteID)
{
    Sprite sprite = {};
    switch (spriteID)
    {
    case SPRITE_WHITE:
    {
        sprite.offset = {0, 0};
        sprite.size = {1, 1};
        break;
    }
    case SPRITE_DICE:
    {
        sprite.offset = {16, 0};
        sprite.size = {16, 16};
        break;
    }
    case SPRITE_CELESTE:
    {
        sprite.offset = {112, 0};
        sprite.size = {17, 20};
        break;
    }
    }
    return sprite;
}