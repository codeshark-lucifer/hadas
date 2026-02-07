#include "interface.h"
#include "utils.h"

RenderData *renderData = nullptr;

Vec2 ScreenToWorld(Input *inputIn)
{
    IVec2 screenPos = inputIn->mousePos; // pivot (0,0) is top-left
    Camera2D camera = renderData->gameCamera;

    // Convert screen pos [0, screenSize] to [-0.5, 0.5]
    float normalizedX = ((float)screenPos.x / inputIn->size.x) - 0.5f;
    float normalizedY = 0.5f - ((float)screenPos.y / inputIn->size.y); // invert Y

    // Scale by camera dimensions
    float xPos = normalizedX * camera.dimentions.x + camera.position.x;
    float yPos = normalizedY * camera.dimentions.y + camera.position.y;

    return {xPos, yPos};
}

void DrawSprite(SpriteID spriteID, Vec2 pos, Vec2 size)
{
    Sprite sprite = getSprite(spriteID);
    Transform transform = {};
    transform.pos = pos;
    transform.size = size;
    transform.ioffset = sprite.offset;
    transform.isize = sprite.size;

    renderData->transforms.push(transform); // use push
}


void DrawSprite(SpriteID spriteID, Vec2 pos)
{
    DrawSprite(spriteID, pos);
}

void DrawQuad(Vec2 pos, Vec2 size)
{
    Transform transform = {};
    transform.pos = pos;
    transform.size = size;
    transform.ioffset = {0, 0};
    transform.isize = {1, 1};

    renderData->transforms.push(transform); 
}

void DrawQuad(Transform transform)
{
    renderData->transforms.push(transform); 
}