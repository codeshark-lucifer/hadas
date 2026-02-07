#pragma once
#include "utils/mathf.h"

enum SpriteID
{
    SPRITE_WHITE,
    SPRITE_DICE,
    SPRITE_COUNT
};

struct Sprite
{
    ivec2 offset;
    ivec2 size;
};

Sprite getSprite(SpriteID spriteID);