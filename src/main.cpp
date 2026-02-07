#include <utils.h>
#include <config.hpp>

Input* input = nullptr;
#include <platform.h>
#include <gl_renderer.hpp>
#include <game.h>
#include <render_types.h> // Include render_types.h to get RenderData struct definition
#include <interface.h>

GameState *gameState = nullptr;

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
    input->size.x = WINDOW_WIDTH;
    input->size.y = WINDOW_HEIGHT;

    WinInfo info = {};
    info.input = input;
    info.title = WINDOW_TITLE;

    RenderData *exeRenderData = (RenderData *)BumpAlloc(&persistentStorage, sizeof(RenderData));
    LOG_ASSERT(exeRenderData, "Failed to allocate RenderData");
    memset(exeRenderData, 0, sizeof(RenderData));

    PlatformFillKeyCodeLookupTable();
    Window window = CreatePlatformWindow(info);
    LOG_ASSERT(window, "Failed to create Window!");
    SetTitleBarColor(window, RGB(25, 25, 25));

    gameState = new GameState();
    LOG_ASSERT(glInit(&transientStorage, exeRenderData), "Failed to initialize OpenGL");
    LOG_DEBUG("EXE renderData = %p", exeRenderData);
    LOG_DEBUG("DLL renderData = %p", exeRenderData);

    while (!ShouldClose())
    {
        ReloadGameDll(&transientStorage);
        WinEvent event;
        PollEvent(&event, input);

        update_game_ptr(gameState, exeRenderData, input);

        glRender(&transientStorage);
        SwapBuffersWindow();

        transientStorage.used = 0;
    }
    delete gameState;
    return EXIT_SUCCESS;
}

void ReloadGameDll(BumpAllocator *transientStroage)
{
    static void *gameDLL;
    static long long lastEditTimestampGameDLL;

    const char *dlllocation = "build/bin/app.dll";

    long long currentTimestampGameDLL = get_timestamp(dlllocation);
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
