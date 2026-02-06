#pragma once
#include "render_types.h" // Include the new header for RenderData and Transform structs
#include "assets.h"

extern RenderData* renderData;

void DrawSprite(SpriteID spriteID, vec2 pos, vec2 size);