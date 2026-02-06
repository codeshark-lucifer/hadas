#pragma once
#include "utils/mathf.h"
#include "utils.h"

enum KeyCodeID
{
    KEY_INVALID = 0,

    KEY_MOUSE_LEFT,
    KEY_MOUSE_MIDDLE,
    KEY_MOUSE_RIGHT,

    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    KEY_SPACE,
    KEY_TICK,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_FORWARD_SLASH,
    KEY_BACKWARD_SLASH,
    KEY_TAB,
    KEY_ESCAPE,
    KEY_PAUSE,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_BACKSPACE,
    KEY_RETURN,
    KEY_DELETE,
    KEY_INSERT,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_CAPS_LOCK,
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_MENU,
    KEY_SHIFT,
    KEY_CONTROL,
    KEY_ALT,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,

    KEY_COUNT
};

struct Key
{
    b8 isDown;
    b8 justPressed;
    b8 justReleased;
    unsigned char halfTransitionCount;
};

struct Input
{
    ivec2 size;

    // Screen
    ivec2 prevMousePos;
    ivec2 mousePos;
    ivec2 relMouse;

    // World
    ivec2 prevMousePosWorld;
    ivec2 mousePosWorld;
    ivec2 relMouseWorld;

    Key keys[KEY_COUNT];
};

extern Input *input;
static KeyCodeID KeyCodeLookupTable[256];

bool IsKeyRelased(KeyCodeID keyCode);
bool IsKeyDown(KeyCodeID keyCode);
bool IsKeyPressed(KeyCodeID keyCode);

// Check if key pressed in this frame
inline bool IsKeyPressed(KeyCodeID keyCode)
{
    Key key = input->keys[keyCode];
    bool result = key.isDown && key.halfTransitionCount == 1 || key.halfTransitionCount > 1;
    return result;
}

// Check if Key release in this frame
inline bool IsKeyRelased(KeyCodeID keyCode)
{
    Key key = input->keys[keyCode];
    bool result = !key.isDown && key.halfTransitionCount == 1 || key.halfTransitionCount > 1;
    return result;
}

// Check if Key down in this frame
inline bool IsKeyDown(KeyCodeID keyCode)
{
    return input->keys[keyCode].isDown;
}