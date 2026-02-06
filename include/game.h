#pragma once
#include "utils.h"
#include "interface.h"
#include "config.hpp"

constexpr const char *WINDOW_TITLE = "hades - engine";
constexpr int WINDOW_WIDTH = 956;
constexpr int WINDOW_HEIGHT = 540;

constexpr int TILESIZE = 8;
constexpr int WORLD_GRID_SIZE = 956;

struct Tile
{
    int neigborMask;
    bool isVisable;
};

struct GameState
{
    Tile worldGrid[WORLD_GRID_SIZE][WORLD_GRID_SIZE];
};

extern GameState *gameState;

extern "C"
{
    __declspec(dllexport) void Update(RenderData *renderDataIn, Input *inputIn);
}