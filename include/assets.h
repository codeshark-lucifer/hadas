#pragma once
#include "utils/mathf.h"

enum SpriteID
{
    SPRITE_WHITE,
    SPRITE_DICE,
    SPRITE_CELESTE,
    
    SPRITE_COUNT
};

struct Sprite
{
    IVec2 offset;
    IVec2 size;
};

Sprite getSprite(SpriteID spriteID);