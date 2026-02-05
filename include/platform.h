#pragma once

#define WIN32_LEAN_AND_MEAN
#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include <windows.h>
#include <GL/glcorearb.h> // Include glcorearb.h first
#include <GL/wglext.h>

typedef HWND Window;

struct WinInfo
{
    int width;
    int height;
    const char* title;
};

struct WinEvent
{
    MSG msg;
};

// Creates a platform-specific window
Window CreatePlatformWindow(const WinInfo& info);

// Polls for window events
void PollEvent(WinEvent* event);

// Global flag to indicate if the application is running
extern bool g_running;
extern HDC hdc;
bool ShouldClose();

void * LoadGLFunc(char* funcName);

void SwapBuffersWindow();