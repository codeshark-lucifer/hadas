#pragma once

#define WIN32_LEAN_AND_MEAN
#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include <windows.h>
#include <GL/glcorearb.h> // Include glcorearb.h first
#include <GL/wglext.h>
#include <config.hpp>

typedef HWND Window;

struct WinInfo
{
    Input* input;
    const char *title;
};

struct WinEvent
{
    MSG msg;
};

extern Window window;

// Creates a platform-specific window
Window CreatePlatformWindow(const WinInfo &info);

// Polls for window events
void PollEvent(WinEvent *event, Input* inputIn);

// Global flag to indicate if the application is running
extern bool g_running;
extern HDC hdc;
bool ShouldClose();

void *LoadGLFunc(const char *funcName);

void SwapBuffersWindow();

bool FreeDynamicLibrary(void *dll);
void *LoadDynamicLibrary(const char *dll);
void *LoadDynamicFunc(void *dll, const char *funcName);

void PlatformFillKeyCodeLookupTable();