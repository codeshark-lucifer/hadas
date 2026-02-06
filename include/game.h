#pragma once
#include "utils.h"
#include "interface.h"
#include "config.hpp"

struct GameState {

};

extern "C"
{
    __declspec(dllexport) void Update(RenderData* renderDataIn, Input* inputIn);
}