#include "platform.h"
#include <cstdio>
#include <utils.h>
#include <GL/glcorearb.h>
#include <interface.h>

bool g_running = false;
HDC hdc = nullptr;
Window window = nullptr;

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
        if (input)
        {
            input->size.x = (float)LOWORD(lParam);
            input->size.y = (float)HIWORD(lParam);
        }
        return 0;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
        LOG_DEBUG("PRESS");
        bool isDown = (uMsg == WM_KEYDOWN) || (uMsg == WM_SYSKEYDOWN) ||
                      (uMsg == WM_LBUTTONDOWN);

        KeyCodeID keyCode = KeyCodeLookupTable[wParam];
        Key *key = &input->keys[keyCode];
        key->justPressed = !key->justPressed && !key->isDown && isDown;
        key->justReleased = !key->justReleased && key->isDown && !isDown;
        key->isDown = isDown;
        key->halfTransitionCount++;
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
    {
        LOG_ERROR("RegisterClassA failed: %lu", GetLastError());
        return nullptr;
    }

    DWORD style = WS_OVERLAPPEDWINDOW;
    RECT rect = {0, 0, info.input->size.x, info.input->size.y};
    AdjustWindowRectEx(&rect, style, FALSE, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    window = CreateWindowExA(
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
    {
        LOG_ERROR("HDC is null");
        return nullptr;
    }

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

    LOG_ASSERT(rc, "wglCreateContext failed");
    LOG_ASSERT(wglMakeCurrent(hdc, rc), "wglMakeCurrent failed");

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    g_running = true;
    return window;
}

void PollEvent(WinEvent *event, Input *inputIn)
{
    LOG_ASSERT(inputIn, "Input is null");

    // Reset per-frame key state
    for (int keyCode = 0; keyCode < KEY_COUNT; keyCode++)
    {
        inputIn->keys[keyCode].justReleased = false;
        inputIn->keys[keyCode].justPressed = false;
        inputIn->keys[keyCode].halfTransitionCount = 0;
    }

    while (PeekMessageA(&event->msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&event->msg);
        DispatchMessageA(&event->msg);
    }

    // Mouse position
    POINT point = {};
    GetCursorPos(&point);
    ScreenToClient(window, &point);

    inputIn->prevMousePos = inputIn->mousePos;
    inputIn->mousePos.x = point.x;
    inputIn->mousePos.y = point.y;
    inputIn->relMouse = inputIn->mousePos - inputIn->prevMousePos;

    vec2 pos = ScreenToWorld(inputIn);
    inputIn->mousePosWorld = ivec2((int)pos.x, (int)pos.y);
}

void *LoadGLFunc(const char *funcName)
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

void *LoadDynamicLibrary(const char *dll)
{
    HMODULE result = LoadLibraryA(dll);
    LOG_ASSERT(result, "Failed to load dll: %s", dll);

    return result;
}

void *LoadDynamicFunc(void *dll, const char *funcName)
{
    FARPROC proc = GetProcAddress((HMODULE)dll, funcName);
    LOG_ASSERT(proc, "Failed to load functions: %s from DLL", funcName);
    return (void *)proc;
}

bool FreeDynamicLibrary(void *dll)
{
    BOOL free = FreeLibrary((HMODULE)dll);
    LOG_ASSERT(free, "Failed to FreeLibrary");
    return (bool)free;
}

void PlatformFillKeyCodeLookupTable()
{
    for (int i = 0; i < 256; i++)
        KeyCodeLookupTable[i] = KEY_INVALID;

    KeyCodeLookupTable[VK_LBUTTON] = KEY_MOUSE_LEFT;
    KeyCodeLookupTable[VK_MBUTTON] = KEY_MOUSE_MIDDLE;
    KeyCodeLookupTable[VK_RBUTTON] = KEY_MOUSE_RIGHT;

    KeyCodeLookupTable['A'] = KEY_A;
    KeyCodeLookupTable['B'] = KEY_B;
    KeyCodeLookupTable['C'] = KEY_C;
    KeyCodeLookupTable['D'] = KEY_D;
    KeyCodeLookupTable['E'] = KEY_E;
    KeyCodeLookupTable['F'] = KEY_F;
    KeyCodeLookupTable['G'] = KEY_G;
    KeyCodeLookupTable['H'] = KEY_H;
    KeyCodeLookupTable['I'] = KEY_I;
    KeyCodeLookupTable['J'] = KEY_J;
    KeyCodeLookupTable['K'] = KEY_K;
    KeyCodeLookupTable['L'] = KEY_L;
    KeyCodeLookupTable['M'] = KEY_M;
    KeyCodeLookupTable['N'] = KEY_N;
    KeyCodeLookupTable['O'] = KEY_O;
    KeyCodeLookupTable['P'] = KEY_P;
    KeyCodeLookupTable['Q'] = KEY_Q;
    KeyCodeLookupTable['R'] = KEY_R;
    KeyCodeLookupTable['S'] = KEY_S;
    KeyCodeLookupTable['T'] = KEY_T;
    KeyCodeLookupTable['U'] = KEY_U;
    KeyCodeLookupTable['V'] = KEY_V;
    KeyCodeLookupTable['W'] = KEY_W;
    KeyCodeLookupTable['X'] = KEY_X;
    KeyCodeLookupTable['Y'] = KEY_Y;
    KeyCodeLookupTable['Z'] = KEY_Z;

    KeyCodeLookupTable['0'] = KEY_0;
    KeyCodeLookupTable['1'] = KEY_1;
    KeyCodeLookupTable['2'] = KEY_2;
    KeyCodeLookupTable['3'] = KEY_3;
    KeyCodeLookupTable['4'] = KEY_4;
    KeyCodeLookupTable['5'] = KEY_5;
    KeyCodeLookupTable['6'] = KEY_6;
    KeyCodeLookupTable['7'] = KEY_7;
    KeyCodeLookupTable['8'] = KEY_8;
    KeyCodeLookupTable['9'] = KEY_9;

    KeyCodeLookupTable[VK_SPACE] = KEY_SPACE;
    KeyCodeLookupTable[VK_TAB] = KEY_TAB;
    KeyCodeLookupTable[VK_ESCAPE] = KEY_ESCAPE;
    KeyCodeLookupTable[VK_RETURN] = KEY_RETURN;
    KeyCodeLookupTable[VK_BACK] = KEY_BACKSPACE;

    KeyCodeLookupTable[VK_SHIFT] = KEY_SHIFT;
    KeyCodeLookupTable[VK_LSHIFT] = KEY_SHIFT;
    KeyCodeLookupTable[VK_RSHIFT] = KEY_SHIFT;

    KeyCodeLookupTable[VK_CONTROL] = KEY_CONTROL;
    KeyCodeLookupTable[VK_MENU] = KEY_ALT;

    KeyCodeLookupTable[VK_F1] = KEY_F1;
    KeyCodeLookupTable[VK_F2] = KEY_F2;
    KeyCodeLookupTable[VK_F3] = KEY_F3;
    KeyCodeLookupTable[VK_F4] = KEY_F4;
    KeyCodeLookupTable[VK_F5] = KEY_F5;
    KeyCodeLookupTable[VK_F6] = KEY_F6;
    KeyCodeLookupTable[VK_F7] = KEY_F7;
    KeyCodeLookupTable[VK_F8] = KEY_F8;
    KeyCodeLookupTable[VK_F9] = KEY_F9;
    KeyCodeLookupTable[VK_F10] = KEY_F10;
    KeyCodeLookupTable[VK_F11] = KEY_F11;
    KeyCodeLookupTable[VK_F12] = KEY_F12;
}
