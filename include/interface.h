#pragma once
#include "render_types.h" // Include the new header for RenderData and Transform structs
#include "assets.h"
#include "config.hpp"

extern RenderData *renderData;

void DrawSprite(SpriteID spriteID, vec2 pos, vec2 size);
void DrawSprite(SpriteID spriteID, vec2 pos);
void DrawQuad(vec2 pos, vec2 size);
void DrawQuad(Transform transform);

vec2 ScreenToWorld(Input *inputIn);