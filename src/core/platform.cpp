#include "platform.h"
#include <cstdio>
#include <utils.h>
#include <GL/glcorearb.h>
#include <config.h>

bool g_running = false;
HDC hdc = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        g_running = false;
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        g_running = false;
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
    {
        screen.width = (float)LOWORD(lParam);
        screen.height = (float)HIWORD(lParam);

        glViewport(0, 0,
                   (GLsizei)screen.width,
                   (GLsizei)screen.height);

        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

HWND CreatePlatformWindow(const WinInfo &info)
{
    HINSTANCE instance = GetModuleHandleA(nullptr);

    WNDCLASSA wc = {};
    wc.hInstance = instance;
    wc.lpszClassName = "MyClassName";
    wc.lpfnWndProc = WindowProc;
    wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassA(&wc))
        return nullptr;

    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rect = {0, 0, info.width, info.height};
    AdjustWindowRectEx(&rect, style, FALSE, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    HWND window = CreateWindowExA(
        0,
        wc.lpszClassName,
        info.title,
        style,
        100, 100,
        width, height,
        nullptr,
        nullptr,
        instance,
        nullptr);

    if (!window)
        return nullptr;

    hdc = GetDC(window);
    if (!hdc)
        return nullptr;

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 24;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!pixelFormat)
        return nullptr;

    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
        return nullptr;

    HGLRC rc = wglCreateContext(hdc);
    if (!rc)
        return nullptr;

    if (!wglMakeCurrent(hdc, rc))
        return nullptr;

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    g_running = true;
    return window;
}

void PollEvent(WinEvent *event)
{
    while (PeekMessageA(&event->msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&event->msg);
        DispatchMessageA(&event->msg);
    }
}

void *LoadGLFunc(char *funcName)
{
    PROC proc = wglGetProcAddress(funcName);
    if (!proc)
    {
        static HMODULE openGLDLL = LoadLibraryA("opengl32.dll");
        proc = GetProcAddress(openGLDLL, funcName);
        if (!proc)
        {
            LOG_ASSERT(false, "Failed to load gl function %s", funcName);
            return nullptr;
        }
    }
    return (void *)proc;
}

bool ShouldClose()
{
    return !g_running;
}

void SwapBuffersWindow()
{
    SwapBuffers(hdc);
}