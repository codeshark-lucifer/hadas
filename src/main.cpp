#include <utils.h>
#include <platform.h>
#include <gl_renderer.hpp>
#include <config.h>
#include <game.h>

ScreenSize screen;
int main()
{
    screen.width = 956;
    screen.height = 540;

    WinInfo info = {};
    info.width = screen.width;
    info.height = screen.height;
    info.title = "Hades";

    Window window = CreatePlatformWindow(info);
    LOG_ASSERT(window, "Failed to create Window!");

    BumpAllocator transientStorage = MakeAllocator(MB(50));

    LOG_ASSERT(glInit(&transientStorage), "Failed to initialize OpenGL");
    while (!ShouldClose())
    {
        WinEvent event;
        PollEvent(&event);
        Update();
        glRender();
        SwapBuffersWindow();
    }
    return EXIT_SUCCESS;
}