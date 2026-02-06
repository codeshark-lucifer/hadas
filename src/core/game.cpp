#include "game.h"

#include "assets.h"
#include "utils.h"

RenderData* renderData; // Global definition for renderData within the DLL

// Update Game Logic
EXPORT_FN void Update(RenderData *renderDataIn, Input *inputIn)
{
    // LOG_DEBUG("game.cpp: renderDataIn = %p", renderDataIn);
    renderData = renderDataIn;
    // LOG_DEBUG("game.cpp: Global renderData set to %p", renderData);
    input = inputIn;
    // LOG_DEBUG("game.cpp: Input set to %p", input);

    DrawSprite(SPRITE_LOGO, {100.0f, 100.0f}, {100.0f, 100.0f});
}
