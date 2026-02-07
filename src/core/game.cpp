#include "game.h"
#include "assets.h"
#include "utils.h"

GameState *gameState = nullptr;
Input *input = nullptr;

// ----------------------
// Grid / Tile helpers
// ----------------------
IVec2 getGridPos(const Vec2 &worldPos)
{
    int x = int((worldPos.x + (WORLD_GRID.x * TILESIZE) / 2.0f) / TILESIZE);
    int y = int((worldPos.y + (WORLD_GRID.y * TILESIZE) / 2.0f) / TILESIZE);
    return {x, y};
}

Tile *getTile(int x, int y)
{
    if (x < 0 || x >= WORLD_GRID.x || y < 0 || y >= WORLD_GRID.y)
        return nullptr;
    return &gameState->worldGrid[x][y];
}

Tile *getTile(const Vec2 &worldPos)
{
    IVec2 gridPos = getGridPos(worldPos);
    return getTile(gridPos.x, gridPos.y);
}

Vec2 getTilePos(int x, int y)
{
    return {
        float(x * TILESIZE) + TILESIZE * 0.5f - (WORLD_GRID.x * TILESIZE) / 2.0f,
        float(y * TILESIZE) + TILESIZE * 0.5f - (WORLD_GRID.y * TILESIZE) / 2.0f
    };
}

// Use IVec2 for size to match IRect definition
IRect getTileRect(int x, int y)
{
    return {getTilePos(x, y), IVec2{TILESIZE, TILESIZE}};
}

// Player rect
IRect getPlayerRect()
{
    return {gameState->player.pos - Vec2{4.0f, 8.0f}, IVec2{8, 16}};
}

// ----------------------
// Game Init
// ----------------------
void InitGame(GameState *state)
{
    IVec2 tilesPos = {48, 0};
    for (int y = 0; y < 5; y++)
        for (int x = 0; x < 4; x++)
            state->tileCoords.push({tilesPos.x + x * 8, tilesPos.y + y * 8});

    state->tileCoords.push({tilesPos.x, tilesPos.y + 5 * 8});
}

// ----------------------
// Simulation
// ----------------------
void simulate()
{
    static Vec2 remainder = {};

    Player &player = gameState->player;
    player.prevPos = player.pos;

    // --- Input ---
    Vec2 moveDir = {};
    float moveSpeed = 100.0f;

    if (IsKeyPressed(KEY_W)) moveDir.y += 1.0f;
    if (IsKeyPressed(KEY_S)) moveDir.y -= 1.0f;
    if (IsKeyPressed(KEY_A)) moveDir.x -= 1.0f;
    if (IsKeyPressed(KEY_D)) moveDir.x += 1.0f;

    // Normalize diagonal
    if (moveDir.x != 0 || moveDir.y != 0)
    {
        float len = sqrtf(moveDir.x * moveDir.x + moveDir.y * moveDir.y);
        moveDir = moveDir / len;
    }

    player.speed = moveDir * moveSpeed * float(UPDATE_DELAY);

    // --- Subpixel movement & collision ---
    remainder += player.speed;
    int moveX = int(round(remainder.x));
    int moveY = int(round(remainder.y));
    remainder.x -= moveX;
    remainder.y -= moveY;

    // Move X
    {
        IRect rect = getPlayerRect();
        rect.pos.x += float(moveX);

        IVec2 gridPos = getGridPos(player.pos);
        bool collision = false;
        for (int x = gridPos.x - 1; x <= gridPos.x + 1; x++)
            for (int y = gridPos.y - 1; y <= gridPos.y + 1; y++)
            {
                Tile *tile = getTile(x, y);
                if (!tile || !tile->isVisible) continue;
                if (rect_collision(rect, getTileRect(x, y))) collision = true;
            }

        if (!collision) player.pos.x += float(moveX);
    }

    // Move Y
    {
        IRect rect = getPlayerRect();
        rect.pos.y += float(moveY);

        IVec2 gridPos = getGridPos(player.pos);
        bool collision = false;
        for (int x = gridPos.x - 1; x <= gridPos.x + 1; x++)
            for (int y = gridPos.y - 1; y <= gridPos.y + 1; y++)
            {
                Tile *tile = getTile(x, y);
                if (!tile || !tile->isVisible) continue;
                if (rect_collision(rect, getTileRect(x, y))) collision = true;
            }

        if (!collision) player.pos.y += float(moveY);
    }

    // --- Mouse interaction ---
    bool updateTile = false;
    if (IsKeyDown(KEY_MOUSE_LEFT))
    {
        Vec2 mouseWorld = Vec2{float(input->mousePosWorld.x), float(input->mousePosWorld.y)};
        Tile *tile = getTile(mouseWorld);
        if (tile) { tile->isVisible = true; updateTile = true; }
    }
    if (IsKeyDown(KEY_MOUSE_RIGHT))
    {
        Vec2 mouseWorld = Vec2{float(input->mousePosWorld.x), float(input->mousePosWorld.y)};
        Tile *tile = getTile(mouseWorld);
        if (tile) { tile->isVisible = false; updateTile = true; }
    }

    // --- Update neighbour mask ---
    if (updateTile)
    {
        int offsets[24] = {
            0,1,-1,0,1,0,0,-1,
            -1,1,1,1,-1,-1,1,-1,
            0,2,-2,0,2,0,0,-2
        };

        for (int y = 0; y < WORLD_GRID.y; y++)
            for (int x = 0; x < WORLD_GRID.x; x++)
            {
                Tile *tile = getTile(x, y);
                if (!tile || !tile->isVisible) continue;

                tile->neighbourMask = 0;
                int neighborCount = 0, extendedCount = 0, emptySlot = 0;

                for (int n = 0; n < 12; n++)
                {
                    Tile *neighbor = getTile(x + offsets[n*2], y + offsets[n*2+1]);
                    if (!neighbor || neighbor->isVisible)
                    {
                        tile->neighbourMask |= BIT(n);
                        if (n < 8) neighborCount++;
                        else extendedCount++;
                    }
                    else if (n < 8) emptySlot = n;
                }

                if (neighborCount == 7 && emptySlot >= 4) tile->neighbourMask = 16 + (emptySlot - 4);
                else if (neighborCount == 8 && extendedCount == 4) tile->neighbourMask = 20;
                else tile->neighbourMask &= 0b1111;
            }
    }
}

// ----------------------
// Update function
// ----------------------
EXPORT_FN void Update(GameState *state, RenderData *renderDataIn, Input *inputIn, float deltaTime)
{
    if (!state->initialized)
    {
        renderData = renderDataIn;
        input = inputIn;
        gameState = state;
        InitGame(gameState);
        state->initialized = true;
    }

    state->updateTimer += deltaTime;
    while (state->updateTimer >= UPDATE_DELAY)
    {
        state->updateTimer -= UPDATE_DELAY;
        simulate();

        input->relMouse = input->mousePos - input->prevMousePos;
        input->prevMousePos = input->mousePos;

        for (int i = 0; i < KEY_COUNT; i++)
        {
            input->keys[i].justPressed = false;
            input->keys[i].justReleased = false;
            input->keys[i].halfTransitionCount = 0;
        }
    }

    float t = float(state->updateTimer / UPDATE_DELAY);
    Vec2 interpPos = mathf::lerp(state->player.prevPos, state->player.pos, t);

    DrawSprite(SPRITE_CELESTE, interpPos, Vec2{50.0f, 50.0f});

    // Draw tiles
    for (int y = 0; y < WORLD_GRID.y; y++)
        for (int x = 0; x < WORLD_GRID.x; x++)
        {
            Tile *tile = getTile(x, y);
            if (!tile || !tile->isVisible) continue;

            Transform tform = {};
            tform.pos = getTilePos(x, y);
            tform.size = Vec2{float(TILESIZE), float(TILESIZE)};
            tform.isize = IVec2{8, 8}; // <-- fix type
            tform.ioffset = state->tileCoords[tile->neighbourMask];

            DrawQuad(tform);
        }
}
