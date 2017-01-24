#pragma once
#ifndef ZP_INPUT_H
#define ZP_INPUT_H

typedef zp_ushort zp_key_code_t;

enum zpKeyCode : zp_key_code_t
{
    ZP_KEY_CODE_NULL = 0x0000,

    ZP_KEY_CODE_MOD_SHIFT = 0x1000,
    ZP_KEY_CODE_MOD_ALT = 0x2000,
    ZP_KEY_CODE_MOD_CTRL = 0x4000,

    ZP_KEY_CODE_BACKSPACE = 0x0008,
    ZP_KEY_CODE_TAB,

    ZP_KEY_CODE_CLEAR = 0x000C,
    ZP_KEY_CODE_RETURN,

    ZP_KEY_CODE_SHIFT = 0x0010,
    ZP_KEY_CODE_CONTROL,
    ZP_KEY_CODE_ALT,
    ZP_KEY_CODE_PAUSE,
    ZP_KEY_CODE_CAPS_LOCK,

    ZP_KEY_CODE_ESC = 0x001B,

    ZP_KEY_CODE_SPACE = 0x0020,
    ZP_KEY_CODE_PAGE_UP,
    ZP_KEY_CODE_PAGE_DOWN,
    ZP_KEY_CODE_END,
    ZP_KEY_CODE_HOME,
    ZP_KEY_CODE_LEFT,
    ZP_KEY_CODE_UP,
    ZP_KEY_CODE_RIGHT,
    ZP_KEY_CODE_DOWN,
    ZP_KEY_CODE_SELECT,
    ZP_KEY_CODE_PRINT,
    ZP_KEY_CODE_EXECUTE,
    ZP_KEY_CODE_PRINT_SCREEN,
    ZP_KEY_CODE_INSERT,
    ZP_KEY_CODE_DELETE,
    ZP_KEY_CODE_HELP,

    ZP_KEY_CODE_0 = 0x0030,
    ZP_KEY_CODE_1,
    ZP_KEY_CODE_2,
    ZP_KEY_CODE_3,
    ZP_KEY_CODE_4,
    ZP_KEY_CODE_5,
    ZP_KEY_CODE_6,
    ZP_KEY_CODE_7,
    ZP_KEY_CODE_8,
    ZP_KEY_CODE_9,

    ZP_KEY_CODE_A = 0x0041,
    ZP_KEY_CODE_B,
    ZP_KEY_CODE_C,
    ZP_KEY_CODE_D,
    ZP_KEY_CODE_E,
    ZP_KEY_CODE_F,
    ZP_KEY_CODE_G,
    ZP_KEY_CODE_H,
    ZP_KEY_CODE_I,
    ZP_KEY_CODE_J,
    ZP_KEY_CODE_K,
    ZP_KEY_CODE_L,
    ZP_KEY_CODE_M,
    ZP_KEY_CODE_N,
    ZP_KEY_CODE_O,
    ZP_KEY_CODE_P,
    ZP_KEY_CODE_Q,
    ZP_KEY_CODE_R,
    ZP_KEY_CODE_S,
    ZP_KEY_CODE_T,
    ZP_KEY_CODE_U,
    ZP_KEY_CODE_V,
    ZP_KEY_CODE_W,
    ZP_KEY_CODE_X,
    ZP_KEY_CODE_Y,
    ZP_KEY_CODE_Z,

    ZP_KEY_CODE_NUMPAD_0 = 0x0060,
    ZP_KEY_CODE_NUMPAD_1,
    ZP_KEY_CODE_NUMPAD_2,
    ZP_KEY_CODE_NUMPAD_3,
    ZP_KEY_CODE_NUMPAD_4,
    ZP_KEY_CODE_NUMPAD_5,
    ZP_KEY_CODE_NUMPAD_6,
    ZP_KEY_CODE_NUMPAD_7,
    ZP_KEY_CODE_NUMPAD_8,
    ZP_KEY_CODE_NUMPAD_9,

    ZP_KEY_CODE_MULTIPLY,
    ZP_KEY_CODE_ADD,
    ZP_KEY_CODE_SEPARATOR,
    ZP_KEY_CODE_SUBTRACT,
    ZP_KEY_CODE_DECIMAL,
    ZP_KEY_CODE_DIVIDE,

    ZP_KEY_CODE_F1 = 0x0070,
    ZP_KEY_CODE_F2,
    ZP_KEY_CODE_F3,
    ZP_KEY_CODE_F4,
    ZP_KEY_CODE_F5,
    ZP_KEY_CODE_F6,
    ZP_KEY_CODE_F7,
    ZP_KEY_CODE_F8,
    ZP_KEY_CODE_F9,
    ZP_KEY_CODE_F10,
    ZP_KEY_CODE_F11,
    ZP_KEY_CODE_F12,
    ZP_KEY_CODE_F13,
    ZP_KEY_CODE_F14,
    ZP_KEY_CODE_F15,
    ZP_KEY_CODE_F16,
    ZP_KEY_CODE_F17,
    ZP_KEY_CODE_F18,
    ZP_KEY_CODE_F19,
    ZP_KEY_CODE_F20,
    ZP_KEY_CODE_F21,
    ZP_KEY_CODE_F22,
    ZP_KEY_CODE_F23,
    ZP_KEY_CODE_F24,

    ZP_KEY_CODE_NUM_LOCK = 0x0090,
    ZP_KEY_CODE_SCROLL_LOCK = 0x0091,

    ZP_KEY_CODE_LSHIFT = 0x00A0,
    ZP_KEY_CODE_RSHIFT,
    ZP_KEY_CODE_LCONTROL,
    ZP_KEY_CODE_RCONTROL,
    ZP_KEY_CODE_LALT,
    ZP_KEY_CODE_RALT,

    ZP_KEY_CODE_BROWSER_BACK = 0x00A6,
    ZP_KEY_CODE_BROWSER_FORWARD,
    ZP_KEY_CODE_BROWSER_REFRESH,
    ZP_KEY_CODE_BROWSER_STOP,
    ZP_KEY_CODE_BROWSER_SEARCH,
    ZP_KEY_CODE_BROWSER_FAVORITES,
    ZP_KEY_CODE_BROWSER_HOME,

    ZP_KEY_CODE_VOLUME_MUTE = 0x00AD,
    ZP_KEY_CODE_VOLUME_DOWN,
    ZP_KEY_CODE_VOLUME_UP,

};

enum
{
    ZP_INPUT_MAX_KEYS = 256
};

#define ZP_KEY_CODE_IS_MODDED( code )           ( 0x7000 & (code) )
#define ZP_KEY_CODE_IS_MOD_DOWN( mod, code )    ( ( ZP_KEY_CODE_MOD_##mod & (code) ) == ZP_KEY_CODE_MOD_##mod )
#define ZP_KEY_CODE_STRIP_MOD( code )           ( 0x0FFF & (code) )

class zpInput
{
public:
    zpInput();
    ~zpInput();

    zp_bool isKeyDown( zp_key_code_t key ) const;
    zp_bool isKeyUp( zp_key_code_t key ) const;
    zp_bool isKeyPressed( zp_key_code_t key ) const;

    zp_char keyToChar( zp_key_code_t key ) const;

    void poll();

    void setKeyState( zp_size_t keyIndex, zp_bool isUp );

private:
    zp_byte m_keyBuffer[ ZP_INPUT_MAX_KEYS ];
    zp_byte m_keyDownBuffer[ ZP_INPUT_MAX_KEYS ];
    zp_byte m_keyPressBuffer[ ZP_INPUT_MAX_KEYS ];
};

#endif // !ZP_INPUT_H
