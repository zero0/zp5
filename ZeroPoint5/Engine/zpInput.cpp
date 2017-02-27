#include "zpEngine.h"

const zp_byte KEY_DOWN = 0xFF;
const zp_byte KEY_UP = 0;

zpInput::zpInput()
    : m_mouseScrollWheel( 0 )
    , m_mouseScrollWheelBuffer( 0 )
    , m_mouseScrollWheelDelta( 0 )
    , m_mouseButtonBuffer( 0 )
    , m_mouseButtonDownBuffer( 0 )
    , m_mouseButtonPressBuffer( 0 )
    , m_mouseLocation()
    , m_mouseLocationBuffer()
    , m_mouseLocationDelta()
{
    zp_memset( m_keyBuffer, 0, sizeof( m_keyBuffer ) );
    zp_memset( m_keyDownBuffer, 0, sizeof( m_keyDownBuffer ) );
    zp_memset( m_keyPressBuffer, 0, sizeof( m_keyPressBuffer ) );
}

zpInput::~zpInput()
{
}

zp_bool zpInput::isKeyDown( zp_key_code_t key ) const
{
    zp_key_code_t code = ZP_KEY_CODE_STRIP_MOD( key );
    return m_keyDownBuffer[ code ] == KEY_DOWN;
}

zp_bool zpInput::isKeyUp( zp_key_code_t key ) const
{
    zp_key_code_t code = ZP_KEY_CODE_STRIP_MOD( key );
    return m_keyDownBuffer[ code ] == KEY_UP;
}

zp_bool zpInput::isKeyPressed( zp_key_code_t key ) const
{
    zp_key_code_t code = ZP_KEY_CODE_STRIP_MOD( key );
    return m_keyPressBuffer[ code ] == KEY_DOWN;
}

zp_char zpInput::keyToChar( zp_key_code_t key ) const
{
    zp_char c = '\0';
    zp_bool caps = ZP_KEY_CODE_IS_MOD_DOWN( SHIFT, key ) || isKeyDown( ZP_KEY_CODE_LSHIFT ) || isKeyDown( ZP_KEY_CODE_RSHIFT );

    zp_key_code_t code = ZP_KEY_CODE_STRIP_MOD( key );
    switch( code )
    {
        case ZP_KEY_CODE_TAB:              c = '\t'; break;

        case ZP_KEY_CODE_RETURN:           c = '\n'; break;

        case ZP_KEY_CODE_SPACE:            c = ' '; break;

        case ZP_KEY_CODE_0:                c = caps ? ')' : '0'; break;
        case ZP_KEY_CODE_1:                c = caps ? '!' : '1'; break;
        case ZP_KEY_CODE_2:                c = caps ? '@' : '2'; break;
        case ZP_KEY_CODE_3:                c = caps ? '#' : '3'; break;
        case ZP_KEY_CODE_4:                c = caps ? '$' : '4'; break;
        case ZP_KEY_CODE_5:                c = caps ? '%' : '5'; break;
        case ZP_KEY_CODE_6:                c = caps ? '^' : '6'; break;
        case ZP_KEY_CODE_7:                c = caps ? '&' : '7'; break;
        case ZP_KEY_CODE_8:                c = caps ? '*' : '8'; break;
        case ZP_KEY_CODE_9:                c = caps ? '(' : '9'; break;

        case ZP_KEY_CODE_A:                c = caps ? 'A' : 'a'; break;
        case ZP_KEY_CODE_B:                c = caps ? 'B' : 'b'; break;
        case ZP_KEY_CODE_C:                c = caps ? 'C' : 'c'; break;
        case ZP_KEY_CODE_D:                c = caps ? 'D' : 'd'; break;
        case ZP_KEY_CODE_E:                c = caps ? 'E' : 'e'; break;
        case ZP_KEY_CODE_F:                c = caps ? 'F' : 'f'; break;
        case ZP_KEY_CODE_G:                c = caps ? 'G' : 'g'; break;
        case ZP_KEY_CODE_H:                c = caps ? 'H' : 'h'; break;
        case ZP_KEY_CODE_I:                c = caps ? 'I' : 'i'; break;
        case ZP_KEY_CODE_J:                c = caps ? 'J' : 'j'; break;
        case ZP_KEY_CODE_K:                c = caps ? 'K' : 'k'; break;
        case ZP_KEY_CODE_L:                c = caps ? 'L' : 'l'; break;
        case ZP_KEY_CODE_M:                c = caps ? 'M' : 'm'; break;
        case ZP_KEY_CODE_N:                c = caps ? 'N' : 'n'; break;
        case ZP_KEY_CODE_O:                c = caps ? 'O' : 'o'; break;
        case ZP_KEY_CODE_P:                c = caps ? 'P' : 'p'; break;
        case ZP_KEY_CODE_Q:                c = caps ? 'Q' : 'q'; break;
        case ZP_KEY_CODE_R:                c = caps ? 'R' : 'r'; break;
        case ZP_KEY_CODE_S:                c = caps ? 'S' : 's'; break;
        case ZP_KEY_CODE_T:                c = caps ? 'T' : 't'; break;
        case ZP_KEY_CODE_U:                c = caps ? 'U' : 'u'; break;
        case ZP_KEY_CODE_V:                c = caps ? 'V' : 'v'; break;
        case ZP_KEY_CODE_W:                c = caps ? 'W' : 'w'; break;
        case ZP_KEY_CODE_X:                c = caps ? 'X' : 'x'; break;
        case ZP_KEY_CODE_Y:                c = caps ? 'Y' : 'y'; break;
        case ZP_KEY_CODE_Z:                c = caps ? 'Z' : 'z'; break;

        case ZP_KEY_CODE_NUMPAD_0:         c = '0'; break;
        case ZP_KEY_CODE_NUMPAD_1:         c = '1'; break;
        case ZP_KEY_CODE_NUMPAD_2:         c = '2'; break;
        case ZP_KEY_CODE_NUMPAD_3:         c = '3'; break;
        case ZP_KEY_CODE_NUMPAD_4:         c = '4'; break;
        case ZP_KEY_CODE_NUMPAD_5:         c = '5'; break;
        case ZP_KEY_CODE_NUMPAD_6:         c = '6'; break;
        case ZP_KEY_CODE_NUMPAD_7:         c = '7'; break;
        case ZP_KEY_CODE_NUMPAD_8:         c = '8'; break;
        case ZP_KEY_CODE_NUMPAD_9:         c = '9'; break;

        case ZP_KEY_CODE_MULTIPLY:         c = '*'; break;
        case ZP_KEY_CODE_ADD:              c = '+'; break;
        case ZP_KEY_CODE_SUBTRACT:         c = '-'; break;
        case ZP_KEY_CODE_DECIMAL:          c = '.'; break;
        case ZP_KEY_CODE_DIVIDE:           c = '/'; break;
    }

    return c;
}

zp_bool zpInput::isMouseButtonDown( zpMouseButton button ) const
{
    return !!( m_mouseButtonDownBuffer & ( 1 << button ) );
}

zp_bool zpInput::isMouseButtonUp( zpMouseButton button ) const
{
    return !( m_mouseButtonDownBuffer & ( 1 << button ) );
}

zp_bool zpInput::isMouseButtonPressed( zpMouseButton button ) const
{
    return !!( m_mouseButtonPressBuffer & ( 1 << button ) );
}

const zpVector2i& zpInput::getMouseLocation() const
{
    return m_mouseLocation;
}

const zpVector2i& zpInput::getMouseLocationDelta() const
{
    return m_mouseLocationDelta;
}

zp_int zpInput::getMouseScrollWheel() const
{
    return m_mouseScrollWheel;
}

zp_int zpInput::getMouseScrollWheelDelta() const
{
    return m_mouseScrollWheelDelta;
}

void zpInput::poll()
{
    // keyboard poll
    for( zp_size_t k = 0; k < ZP_INPUT_MAX_KEYS; ++k )
    {
        m_keyPressBuffer[ k ] = KEY_UP;

        if( m_keyBuffer[ k ] == KEY_DOWN )
        {
            if( m_keyDownBuffer[ k ] == KEY_DOWN )
            {
                // repeat
            }
            else
            {
                // down
            }

            m_keyDownBuffer[ k ] = KEY_DOWN;
        }
        else if( m_keyDownBuffer[ k ] == KEY_DOWN )
        {
            m_keyDownBuffer[ k ] = KEY_UP;
            m_keyPressBuffer[ k ] = KEY_DOWN;

            // up
        }
    }


    // mouse poll
    m_mouseButtonPressBuffer = 0;
    for( zp_uint b = 0; b < zpMouseButton_Count; ++b )
    {
        zp_uint buttonMask = ( 1 << b );
        if( m_mouseButtonBuffer & buttonMask )
        {
            if( m_mouseButtonDownBuffer & buttonMask )
            {
                // repeat
            }
            else
            {
                // down
            }

            m_mouseButtonDownBuffer |= buttonMask;
        }
        else if( m_mouseButtonDownBuffer & b )
        {
            // up
            m_mouseButtonDownBuffer &= ~b;
            m_mouseButtonPressBuffer |= b;
        }
    }

    if( m_mouseScrollWheelBuffer != 0 )
    {
        m_mouseScrollWheelDelta = m_mouseScrollWheelBuffer;
        m_mouseScrollWheel += m_mouseScrollWheelBuffer;

        m_mouseScrollWheelBuffer = 0;
    }
    else
    {
        m_mouseScrollWheelDelta = 0;
    }

    if( m_mouseLocationBuffer.x || m_mouseLocationBuffer.y )
    {
        m_mouseLocationDelta.x = m_mouseLocationBuffer.x - m_mouseLocation.x;
        m_mouseLocationDelta.y = m_mouseLocationBuffer.y - m_mouseLocation.y;

        m_mouseLocation = m_mouseLocationBuffer;

        m_mouseLocationBuffer.x = 0;
        m_mouseLocationBuffer.y = 0;
    }
    else
    {
        m_mouseLocationDelta.x = 0;
        m_mouseLocationDelta.y = 0;
    }
}

void zpInput::setKeyState( zp_size_t keyIndex, zp_bool isUp )
{
    m_keyBuffer[ keyIndex ] = isUp ? KEY_UP : KEY_DOWN;
}

void zpInput::setMouseButtonState( zpMouseButton button, zp_bool isUp )
{
    if( isUp )
    {
        m_mouseButtonBuffer |= ( 1 << button );
    }
    else
    {
        m_mouseButtonBuffer &= ~( 1 << button );
    }
}

void zpInput::setMouseLocationState( zp_int x, zp_int y )
{
    m_mouseLocationBuffer.x = x;
    m_mouseLocationBuffer.y = y;
}

void zpInput::setMouseScrollWheelState( zp_int scroll )
{
    m_mouseScrollWheelBuffer = scroll;
}
