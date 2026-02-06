#pragma once
#include "render_types.h" // Include the new header for RenderData and Transform structs
#include "assets.h"
#include "config.hpp"

extern RenderData *renderData;

void DrawSprite(SpriteID spriteID, vec2 pos, vec2 size);
void DrawQuad(vec2 pos, vec2 size);

vec2 ScreenToWorld(Input *inputIn);