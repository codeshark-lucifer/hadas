#pragma once
#include "utils/mathf.h"
#include "assets.h"
#include "utils.h"

constexpr int MAX_TRANSFORMS = 1000;

struct Camera2D
{
    float zoom{1.0f};
    vec2 dimention{956.0f, 540.0f}; // screen width , hright
    vec2 position{0.0f, 0.0f};
    mat4 matrix()
    {
        mat4 p, v;

        p = mat4::Ortho(
            dimention.x * 0.5f * zoom,
            -dimention.x * 0.5f * zoom,
            dimention.y * 0.5f * zoom,
            -dimention.y * 0.5f * zoom,
            -1, 1);

        // camera moves opposite of the world
        v = mat4::Translate({-position.x, -position.y, 0.0f});

        return p * v;
    }
};

struct Transform
{
    ivec2 ioffset;
    ivec2 isize;

    vec2 pos;
    vec2 size;
};

struct RenderData
{
    Camera2D gameCamera;
    Camera2D uiCamera;

    int transformCount;
    Transform transforms[MAX_TRANSFORMS];
};

