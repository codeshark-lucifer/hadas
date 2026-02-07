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

void InitGame(GameState *gameState)
{
    {
        ivec2 tilesPos = {48, 0};
        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                gameState->tileCoords.push({tilesPos.x + x * 8, tilesPos.y + y * 8});
            }
        }

        gameState->tileCoords.push({tilesPos.x, tilesPos.y + 5 * 8});
    }
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
            InitGame(gameState);
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
        int neigbourOffsets[24] = {
            0, 1, -1, 0, 1, 0, 0, -1,   // 8-direction neighbors
            -1, 1, 1, 1, -1, -1, 1, -1, // extended neighbors
            0, 2, -2, 0, 2, 0, 0, -2};

        for (int y = 0; y < WORLD_GRID.y; y++)
        {
            for (int x = 0; x < WORLD_GRID.x; x++)
            {
                Tile *tile = getTile(x, y);
                if (!tile || !tile->isVisible)
                    continue;

                tile->neigbourMask = 0;
                int neighborCount = 0;
                int extendedNeighborCount = 0;
                int emptyNeighbourSlot = 0;

                for (int n = 0; n < 12; n++)
                {
                    Tile *neighbor = getTile(x + neigbourOffsets[n * 2],
                                             y + neigbourOffsets[n * 2 + 1]);

                    if (!neighbor || neighbor->isVisible)
                    {
                        tile->neigbourMask |= BIT(n);
                        if (n < 8)
                        {
                            neighborCount++;
                        }
                        else
                        {
                            extendedNeighborCount++;
                        }
                    }
                    else if (n < 8)
                    {
                        emptyNeighbourSlot = n;
                    }
                }

                if (neighborCount == 7 && emptyNeighbourSlot >= 4)
                {
                    tile->neigbourMask = 16 + (emptyNeighbourSlot - 4);
                }
                else if (neighborCount == 8 && extendedNeighborCount == 4)
                {
                    tile->neigbourMask = 20;
                }
                else
                {
                    tile->neigbourMask = tile->neigbourMask & 0b1111;
                }

                Transform transform = {};
                transform.pos = {
                    (float)x * TILESIZE + TILESIZE * 0.5f - (WORLD_GRID.x * TILESIZE) / 2.0f,
                    (float)y * TILESIZE + TILESIZE * 0.5f - (WORLD_GRID.y * TILESIZE) / 2.0f};

                transform.size = {TILESIZE, TILESIZE};
                transform.isize = {8, 8};
                transform.ioffset = gameState->tileCoords[tile->neigbourMask];
                DrawQuad(transform);
            }
        }
    }
}
