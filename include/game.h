#pragma once
#include "utils.h"
#include "interface.h"
#include "config.hpp"

constexpr const char *WINDOW_TITLE = "hades - engine";
constexpr int WINDOW_WIDTH = 956;
constexpr int WINDOW_HEIGHT = 540;

constexpr ivec2 WORLD_GRID = {32, 18}; // 32x18 tiles
constexpr int TILESIZE = 32;           // each tile = 32x32 pixels on screen

struct Tile
{
    int neigborMask{0};
    bool isVisible{false};
};

struct GameState
{
    bool initalized = false;
    Tile worldGrid[WORLD_GRID.x][WORLD_GRID.y];
    GameState()
    {
        for (int y = 0; y < WORLD_GRID.y; y++)
        {
            for (int x = 0; x < WORLD_GRID.x; x++)
            {
                worldGrid[x][y] = Tile{};
            }
        }
    }
};

extern GameState *gameState;

extern "C"
{
    __declspec(dllexport) void Update(GameState *gameState, RenderData *renderDataIn, Input *inputIn);
}