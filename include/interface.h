#pragma once
#include "utils/mathf.h"

constexpr int MAX_TRANSFORMS = 1000;

struct Transform
{
    ivec2 ioffset;
    ivec2 isize;

    vec2 pos;
    vec2 size;
};

