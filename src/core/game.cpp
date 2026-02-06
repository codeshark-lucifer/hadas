#include "game.h"

#include "assets.h"
#include "utils.h"

Input *input = nullptr;
GameState *gameState = nullptr;


// Update Game Logic
EXPORT_FN void Update(RenderData *renderDataIn, Input *inputIn)
{
    if (renderData != renderDataIn)
    {
        renderData = renderDataIn;
        input = inputIn;
    }
    if (input && renderData)
    {
        renderData->gameCamera.dimention = {
            float(input->size.x),
            float(input->size.y)};
        renderData->gameCamera.zoom = 1.0f;
    }
    DrawSprite(
        SPRITE_LOGO,
        {0.0f, 0.0f},
        {100.0f, 100.0f});

}
