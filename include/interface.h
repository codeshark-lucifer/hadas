#pragma once
#include "render_types.h" // Include the new header for RenderData and Transform structs
#include "assets.h"
#include "config.hpp"

extern RenderData *renderData;

void DrawSprite(SpriteID spriteID, Vec2 pos, Vec2 size);
void DrawSprite(SpriteID spriteID, Vec2 pos);
void DrawQuad(Vec2 pos, Vec2 size);
void DrawQuad(Transform transform);

Vec2 ScreenToWorld(Input *inputIn);