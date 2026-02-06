#include "interface.h"
#include "utils.h"

RenderData *renderData = nullptr;

vec2 ScreenToWorld(Input *inputIn)
{
    ivec2 screenPos = inputIn->mousePos;
    Camera2D camera = renderData->gameCamera;

    float xPos = (float)screenPos.x /
                 inputIn->size.x /
                 camera.dimention.x;

    xPos += -camera.dimention.x / 2.0f + camera.position.x;

    float yPos = (float)screenPos.y /
                 inputIn->size.y /
                 camera.dimention.y;

    yPos += camera.dimention.y / 2.0f - camera.position.y;

    return {xPos, yPos};
}

void DrawSprite(SpriteID spriteID, vec2 pos, vec2 size)
{
    // LOG_DEBUG("interface.cpp: DrawSprite - renderData = %p", renderData);
    LOG_ASSERT(renderData != nullptr, "renderData not initialized!");
    LOG_ASSERT(renderData->transformCount < MAX_TRANSFORMS, "Too many transforms!");
    if (renderData->transformCount >= MAX_TRANSFORMS)
        return;

    Sprite sprite = getSprite(spriteID);
    Transform transform = {};
    transform.pos = pos;
    transform.size = size;
    transform.ioffset = sprite.offset;
    transform.isize = sprite.size;

    renderData->transforms[renderData->transformCount++] = transform;
}

void DrawQuad(vec2 pos, vec2 size)
{
    DrawSprite(SPRITE_LOGO, pos, size);
}