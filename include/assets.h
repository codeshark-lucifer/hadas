#pragma once
#include "utils/mathf.h"

enum SpriteID
{
    SPRITE_LOGO,
    SPRITE_COUNT
};

struct Sprite
{
    ivec2 offset;
    ivec2 size;
};

Sprite getSprite(SpriteID spriteID);