#include "game.h"

#include "assets.h"
#include "utils.h"

Input *input = nullptr;
GameState *gameState = nullptr;

Tile *getTile(int x, int y)
{
    Tile *tile = nullptr;

    if (x >= 0 && x < WORLD_GRID.x &&
        y >= 0 && y < WORLD_GRID.y)
    {
        tile = &gameState->worldGrid[x][y];
    }
    return tile;
}

Tile *getTile(ivec2 worldPos)
{
    int x = (worldPos.x + (WORLD_GRID.x * TILESIZE) / 2) / TILESIZE;
    int y = (worldPos.y + (WORLD_GRID.y * TILESIZE) / 2) / TILESIZE;

    return getTile(x, y);
}

// Update Game Logic
EXPORT_FN void Update(GameState *gameState, RenderData *renderDataIn, Input *inputIn)
{
    if (!gameState->initalized)
    {
        if (renderData != renderDataIn)
        {
            renderData = renderDataIn;
            input = inputIn;

            ::gameState = gameState; // assign to global
            gameState->initalized = true;
        }
    }
    DrawSprite(
        SPRITE_DICE,
        {(float)input->mousePosWorld.x, (float)input->mousePosWorld.y},
        {50.0f, 50.0f});

    if (IsKeyDown(KEY_MOUSE_LEFT))
    {
        ivec2 worldPos = input->mousePosWorld;
        Tile *tile = getTile(worldPos);
        if (tile)
            tile->isVisible = true;
    }
    
    if (IsKeyDown(KEY_MOUSE_RIGHT))
    {
        ivec2 worldPos = input->mousePosWorld;
        Tile *tile = getTile(worldPos);
        if (tile)
            tile->isVisible = false;
    }

    {
        for (int y = 0; y < WORLD_GRID.y; y++)
        {
            for (int x = 0; x < WORLD_GRID.x; x++)
            {
                Tile *tile = getTile(x, y);
                if (!tile || !tile->isVisible)
                    continue;

                vec2 pos = {
                    (float)x * TILESIZE + TILESIZE * 0.5f - (WORLD_GRID.x * TILESIZE) / 2.0f,
                    (float)y * TILESIZE + TILESIZE * 0.5f - (WORLD_GRID.y * TILESIZE) / 2.0f};

                DrawSprite(SPRITE_DICE, pos, vec2((float)TILESIZE));
            }
        }
    }
}
