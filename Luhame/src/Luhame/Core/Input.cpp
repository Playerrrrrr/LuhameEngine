#include"pch.h"
#include"Input.h"
#include"GLFW/glfw3.h"
#include"imgui.h"
namespace Luhame {

    std::unordered_map<int, int> input::key_map = {
        // ×ÖÄ¸¼ü
        {LH_KEY_A, ImGuiKey_A},
        {LH_KEY_B, ImGuiKey_B},
        {LH_KEY_C, ImGuiKey_C},
        {LH_KEY_D, ImGuiKey_D},
        {LH_KEY_E, ImGuiKey_E},
        {LH_KEY_F, ImGuiKey_F},
        {LH_KEY_G, ImGuiKey_G},
        {LH_KEY_H, ImGuiKey_H},
        {LH_KEY_I, ImGuiKey_I},
        {LH_KEY_J, ImGuiKey_J},
        {LH_KEY_K, ImGuiKey_K},
        {LH_KEY_L, ImGuiKey_L},
        {LH_KEY_M, ImGuiKey_M},
        {LH_KEY_N, ImGuiKey_N},
        {LH_KEY_O, ImGuiKey_O},
        {LH_KEY_P, ImGuiKey_P},
        {LH_KEY_Q, ImGuiKey_Q},
        {LH_KEY_R, ImGuiKey_R},
        {LH_KEY_S, ImGuiKey_S},
        {LH_KEY_T, ImGuiKey_T},
        {LH_KEY_U, ImGuiKey_U},
        {LH_KEY_V, ImGuiKey_V},
        {LH_KEY_W, ImGuiKey_W},
        {LH_KEY_X, ImGuiKey_X},
        {LH_KEY_Y, ImGuiKey_Y},
        {LH_KEY_Z, ImGuiKey_Z},

        // Êý×Ö¼ü
        {LH_KEY_0, ImGuiKey_0},
        {LH_KEY_1, ImGuiKey_1},
        {LH_KEY_2, ImGuiKey_2},
        {LH_KEY_3, ImGuiKey_3},
        {LH_KEY_4, ImGuiKey_4},
        {LH_KEY_5, ImGuiKey_5},
        {LH_KEY_6, ImGuiKey_6},
        {LH_KEY_7, ImGuiKey_7},
        {LH_KEY_8, ImGuiKey_8},
        {LH_KEY_9, ImGuiKey_9},

        // ¹¦ÄÜ¼ü
        {LH_KEY_F1, ImGuiKey_F1},
        {LH_KEY_F2, ImGuiKey_F2},
        {LH_KEY_F3, ImGuiKey_F3},
        {LH_KEY_F4, ImGuiKey_F4},
        {LH_KEY_F5, ImGuiKey_F5},
        {LH_KEY_F6, ImGuiKey_F6},
        {LH_KEY_F7, ImGuiKey_F7},
        {LH_KEY_F8, ImGuiKey_F8},
        {LH_KEY_F9, ImGuiKey_F9},
        {LH_KEY_F10, ImGuiKey_F10},
        {LH_KEY_F11, ImGuiKey_F11},
        {LH_KEY_F12, ImGuiKey_F12},
        {LH_KEY_F13, ImGuiKey_F13},
        {LH_KEY_F14, ImGuiKey_F14},
        {LH_KEY_F15, ImGuiKey_F15},
        {LH_KEY_F16, ImGuiKey_F16},
        {LH_KEY_F17, ImGuiKey_F17},
        {LH_KEY_F18, ImGuiKey_F18},
        {LH_KEY_F19, ImGuiKey_F19},
        {LH_KEY_F20, ImGuiKey_F20},
        {LH_KEY_F21, ImGuiKey_F21},
        {LH_KEY_F22, ImGuiKey_F22},
        {LH_KEY_F23, ImGuiKey_F23},
        {LH_KEY_F24, ImGuiKey_F24},

        // µ¼º½¼ü
        {LH_KEY_UP, ImGuiKey_UpArrow},
        {LH_KEY_DOWN, ImGuiKey_DownArrow},
        {LH_KEY_LEFT, ImGuiKey_LeftArrow},
        {LH_KEY_RIGHT, ImGuiKey_RightArrow},
        {LH_KEY_HOME, ImGuiKey_Home},
        {LH_KEY_END, ImGuiKey_End},
        {LH_KEY_PAGE_UP, ImGuiKey_PageUp},
        {LH_KEY_PAGE_DOWN, ImGuiKey_PageDown},
        {LH_KEY_INSERT, ImGuiKey_Insert},
        {LH_KEY_DELETE, ImGuiKey_Delete},

        // ±à¼­¼ü
        {LH_KEY_BACKSPACE, ImGuiKey_Backspace},
        {LH_KEY_ENTER, ImGuiKey_Enter},
        {LH_KEY_ESCAPE, ImGuiKey_Escape},
        {LH_KEY_TAB, ImGuiKey_Tab},
        {LH_KEY_SPACE, ImGuiKey_Space},

        // ·ûºÅ¼ü
        {LH_KEY_APOSTROPHE, ImGuiKey_Apostrophe},
        {LH_KEY_COMMA, ImGuiKey_Comma},
        {LH_KEY_MINUS, ImGuiKey_Minus},
        {LH_KEY_PERIOD, ImGuiKey_Period},
        {LH_KEY_SLASH, ImGuiKey_Slash},
        {LH_KEY_SEMICOLON, ImGuiKey_Semicolon},
        {LH_KEY_EQUAL, ImGuiKey_Equal},
        {LH_KEY_LEFT_BRACKET, ImGuiKey_LeftBracket},
        {LH_KEY_BACKSLASH, ImGuiKey_Backslash},
        {LH_KEY_RIGHT_BRACKET, ImGuiKey_RightBracket},
        {LH_KEY_GRAVE_ACCENT, ImGuiKey_GraveAccent},

        // ÐÞÊÎ¼ü
        {LH_KEY_LEFT_SHIFT, ImGuiKey_LeftShift},
        {LH_KEY_LEFT_CONTROL, ImGuiKey_LeftCtrl},
        {LH_KEY_LEFT_ALT, ImGuiKey_LeftAlt},
        {LH_KEY_LEFT_SUPER, ImGuiKey_LeftSuper},
        {LH_KEY_RIGHT_SHIFT, ImGuiKey_RightShift},
        {LH_KEY_RIGHT_CONTROL, ImGuiKey_RightCtrl},
        {LH_KEY_RIGHT_ALT, ImGuiKey_RightAlt},
        {LH_KEY_RIGHT_SUPER, ImGuiKey_RightSuper},

        // Êý×ÖÐ¡¼üÅÌ
        {LH_KEY_KP_0, ImGuiKey_Keypad0},
        {LH_KEY_KP_1, ImGuiKey_Keypad1},
        {LH_KEY_KP_2, ImGuiKey_Keypad2},
        {LH_KEY_KP_3, ImGuiKey_Keypad3},
        {LH_KEY_KP_4, ImGuiKey_Keypad4},
        {LH_KEY_KP_5, ImGuiKey_Keypad5},
        {LH_KEY_KP_6, ImGuiKey_Keypad6},
        {LH_KEY_KP_7, ImGuiKey_Keypad7},
        {LH_KEY_KP_8, ImGuiKey_Keypad8},
        {LH_KEY_KP_9, ImGuiKey_Keypad9},
        {LH_KEY_KP_DECIMAL, ImGuiKey_KeypadDecimal},
        {LH_KEY_KP_DIVIDE, ImGuiKey_KeypadDivide},
        {LH_KEY_KP_MULTIPLY, ImGuiKey_KeypadMultiply},
        {LH_KEY_KP_SUBTRACT, ImGuiKey_KeypadSubtract},
        {LH_KEY_KP_ADD, ImGuiKey_KeypadAdd},
        {LH_KEY_KP_ENTER, ImGuiKey_KeypadEnter},
        {LH_KEY_KP_EQUAL, ImGuiKey_KeypadEqual},

        // ÆäËû¼ü
        {LH_KEY_MENU, ImGuiKey_Menu},
        {LH_KEY_CAPS_LOCK, ImGuiKey_CapsLock},
        {LH_KEY_SCROLL_LOCK, ImGuiKey_ScrollLock},
        {LH_KEY_NUM_LOCK, ImGuiKey_NumLock},
        {LH_KEY_PRINT_SCREEN, ImGuiKey_PrintScreen},
        {LH_KEY_PAUSE, ImGuiKey_Pause}
    };

}