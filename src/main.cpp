#include <utils.h>
#include <config.hpp>

#include <platform.h>
#include <gl_renderer.hpp>
#include <game.h>
#include <render_types.h> // Include render_types.h to get RenderData struct definition

typedef decltype(Update) UpdateGameType;
static UpdateGameType *update_game_ptr;

void ReloadGameDll(BumpAllocator *transientStroage);

int main()
{
    BumpAllocator transientStorage = MakeAllocator(MB(50));
    BumpAllocator persistentStorage = MakeAllocator(MB(50));

    input = (Input *)BumpAlloc(&persistentStorage, sizeof(Input));
    LOG_ASSERT(input, "Failed to allocate Input");
    memset(input, 0, sizeof(Input));
    input->size.x = 956;
    input->size.y = 540;

    WinInfo info = {};
    info.width = input->size.x;
    info.height = input->size.y;
    info.title = "Hades";

    RenderData* exeRenderData = (RenderData *)BumpAlloc(&persistentStorage, sizeof(RenderData));
    LOG_ASSERT(exeRenderData, "Failed to allocate RenderData");
    memset(exeRenderData, 0, sizeof(RenderData));

    Window window = CreatePlatformWindow(info);
    LOG_ASSERT(window, "Failed to create Window!");

    LOG_ASSERT(glInit(&transientStorage, exeRenderData), "Failed to initialize OpenGL");
    LOG_DEBUG("EXE renderData = %p", exeRenderData);
    LOG_DEBUG("DLL renderData = %p", exeRenderData);

    while (!ShouldClose())
    {
        ReloadGameDll(&transientStorage);
        exeRenderData->transformCount = 0;

        WinEvent event;
        PollEvent(&event);

        update_game_ptr(exeRenderData, input);

        glRender();
        SwapBuffersWindow();

        transientStorage.used = 0;
    }

    return EXIT_SUCCESS;
}

void ReloadGameDll(BumpAllocator *transientStroage)
{
    static void *gameDLL;
    static long long lastEditTimestampGameDLL;

    const char *dlllocation = "build/bin/app.dll";

    long long currentTimestampGameDLL = get_timestemp(dlllocation);
    if (currentTimestampGameDLL > lastEditTimestampGameDLL)
    {
        if (gameDLL)
        {
            bool freeeResult = FreeDynamicLibrary(gameDLL);
            LOG_ASSERT(freeeResult, "Failed to free app.dll");
            gameDLL = nullptr;
            LOG_DEBUG("Free app.dll");
        }
        while (copy_file(dlllocation, "load.dll", transientStroage))
        {
            Sleep(10);
        }
        LOG_DEBUG("Copied DLL file.");

        gameDLL = LoadDynamicLibrary("load.dll");
        LOG_ASSERT(gameDLL, "Failed to load load.dll");

        update_game_ptr = (UpdateGameType *)LoadDynamicFunc(gameDLL, "Update");
        LOG_ASSERT(update_game_ptr, "Failed to load Update() function");
        lastEditTimestampGameDLL = currentTimestampGameDLL;
    }
}
