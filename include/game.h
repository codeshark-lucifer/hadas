#pragma once
#include "utils.h"
#include "interface.h"
#include "config.hpp"

constexpr int UPDATE_PER_SECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / UPDATE_PER_SECOND;

constexpr const char *WINDOW_TITLE = "hades - engine";
constexpr int WINDOW_WIDTH = 956;
constexpr int WINDOW_HEIGHT = 540;

constexpr IVec2 WORLD_GRID = {32, 18}; // 32x18 tiles
constexpr int TILESIZE = 32;           // each tile = 32x32 pixels on screen

struct Tile
{
    int neighbourMask{0};
    bool isVisible{false};
};

struct Player
{
    Vec2 pos;
    Vec2 prevPos;
    Vec2 speed;
};

struct GameState
{
    float updateTimer = 0;
    bool initialized = false;

    Player player;
    Array<IVec2> tileCoords;
    Tile worldGrid[WORLD_GRID.x][WORLD_GRID.y];

    GameState()
    {
        for (int y = 0; y < WORLD_GRID.y; y++)
            for (int x = 0; x < WORLD_GRID.x; x++)
                worldGrid[x][y] = Tile{};
    }
};

extern GameState *gameState;
extern Input *input;

extern "C"
{
    __declspec(dllexport) void Update(GameState *gameState, RenderData *renderDataIn, Input *inputIn, float deltaTime);
}

// Grid / Tile helpers
IVec2 getGridPos(const Vec2 &worldPos);
Tile *getTile(int x, int y);
Tile *getTile(const Vec2 &worldPos);
IRect getPlayerRect();
Vec2 getTilePos(int x, int y);
IRect getTileRect(int x, int y);

void InitGame(GameState *gameState);
