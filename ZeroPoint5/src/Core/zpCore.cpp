#include "Core/zpCore.h"
#include <memory>
#include <stdarg.h>

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // !ZP_WINDOWS

enum
{
    ZP_PRINT_BUFFER_SIZE =      1024,
    ZP_ASSERT_TITLE_SIZE =      256,
    ZP_ASSERT_MESSAGE_SIZE =    512,
};

void* zp_malloc( zp_size_t size )
{
    return malloc( size );
}

void zp_free( void* ptr )
{
    free( ptr );
}

void zp_sleep( zp_int milliseconds )
{
    Sleep( milliseconds );
}

void* zp_memcpy( void* dest, zp_size_t destSize, const void* src, zp_size_t size )
{
#ifdef ZP_USE_SAFE_FUNCTIONS
    memcpy_s( dest, destSize, src, size );
    return dest;
#else
    ZP_UNUSED( destSize );
    return memcpy( dest, src, size );
#endif
}

void* zp_memset( void* dest, zp_int value, zp_size_t size )
{
    return memset( dest, value, size );
}

zp_int zp_memcmp( const void* ptr1, const void* ptr2, zp_size_t size )
{
    return memcmp( ptr1, ptr2, size );
}

zp_char* zp_strcpy( zp_char* destString, zp_size_t numElements, const zp_char* srcString )
{
#ifdef ZP_USE_SAFE_FUNCTIONS
    return strcpy_s( destString, numElements, srcString ) == 0 ? destString : ZP_NULL;
#else
    return strcpy( destString, numElements, srcString );
#endif
}

zp_char* zp_strncpy( zp_char* destString, zp_size_t numElements, const zp_char* srcString, zp_size_t maxCount )
{
#ifdef ZP_USE_SAFE_FUNCTIONS
    strncpy_s( destString, numElements, srcString, maxCount );
    return destString;
#else
    ZP_UNUSED( numElements );
    return strncpy( destString, srcString, maxCount );
#endif
}

zp_size_t zp_strlen( const zp_char* srcString )
{
    return srcString ? strlen( srcString ) : 0;
}

zp_int zp_strcmp( const zp_char* str1, const zp_char* str2 )
{
    return strcmp( str1, str2 );
}

zp_int zp_stricmp( const zp_char* str1, const zp_char* str2 )
{
    return _stricmp( str1, str2 );
}

#ifdef ZP_USE_ASSERTIONS
void zp_assert( const zp_char* file, zp_int line, const zp_char* msg, ... )
{
    zp_char message[ ZP_ASSERT_MESSAGE_SIZE ];
    zp_char text[ ZP_ASSERT_MESSAGE_SIZE ];
    zp_char title[ ZP_ASSERT_TITLE_SIZE ];

    va_list vl;
    va_start( vl, msg );
#ifdef ZP_USE_SAFE_FUNCTIONS
    vsnprintf_s( message, sizeof( message ), sizeof( message ), msg, vl );
#else // !ZP_USE_SAFE_FUNCTIONS
    vsnprintf( message, sizeof( message ), msg, vl );
#endif // ZP_USE_SAFE_FUNCTIONS
    va_end( vl );

#ifdef ZP_USE_SAFE_FUNCTIONS
    _snprintf_s( text, sizeof( text ), sizeof( text ), "%s\n\nDebug Break (Abort)  Continue (Retry)  Ignore", message );
    _snprintf_s( title, sizeof( title ), sizeof( title ), "ZeroPoint Assert Failed at %s:%d", file, line );
#else // !ZP_USE_SAFE_FUNCTIONS
    snprintf( text, sizeof( text ), "%s\n\nDebug Break (Abort)  Continue (Retry)  Ignore", message );
    snprintf( title, sizeof( title ), "ZeroPoint Assert Failed at %s:%d", file, line );
#endif // ZP_USE_SAFE_FUNCTIONS

#ifdef ZP_WINDOWS
    zp_int result = MessageBox( ZP_NULL, text, title, MB_ABORTRETRYIGNORE | MB_ICONERROR );
    switch( result )
    {
        case IDOK:
        case IDYES:
        case IDABORT:
            DebugBreak();
            break;
        case IDNO:
        case IDRETRY:
            // continue
            break;
        case IDCANCEL:
        case IDIGNORE:
            // ignore
            break;
    }
#endif // !ZP_WINDOWS
}

void zp_assert_warning( const zp_char* file, zp_int line, const zp_char* msg, ... )
{
    zp_char message[ ZP_ASSERT_MESSAGE_SIZE ];
    zp_char text[ ZP_ASSERT_MESSAGE_SIZE ];
    zp_char title[ ZP_ASSERT_TITLE_SIZE ];

    va_list vl;
    va_start( vl, msg );
#ifdef ZP_USE_SAFE_FUNCTIONS
    vsnprintf_s( message, sizeof( message ), sizeof( message ), msg, vl );
#else // !ZP_USE_SAFE_FUNCTIONS
    vsnprintf( message, sizeof( message ), msg, vl );
#endif // ZP_USE_SAFE_FUNCTIONS
    va_end( vl );

#ifdef ZP_USE_SAFE_FUNCTIONS
    _snprintf_s( text, sizeof( text ), sizeof( text ), "%s\n\nDebug Break (Retry)  Continue (Cancel)", message );
    _snprintf_s( title, sizeof( title ), sizeof( title ), "ZeroPoint Assert Warning at %s:%d", file, line );
#else // !ZP_USE_SAFE_FUNCTIONS
    snprintf( text, sizeof( text ), "%s\n\nDebug Break (Retry)  Continue (Cancel)", message );
    snprintf( title, sizeof( title ), "ZeroPoint Assert Warning at %s:%d", file, line );
#endif // ZP_USE_SAFE_FUNCTIONS

#ifdef ZP_WINDOWS
    zp_int result = MessageBox( ZP_NULL, text, title, MB_RETRYCANCEL | MB_ICONWARNING );
    switch( result )
    {
        case IDRETRY:
            DebugBreak();
            break;
        case IDCANCEL:
            // ignore
            break;
    }
#endif // !ZP_WINDOWS
}
#endif // !ZP_USE_ASSERTIONS

#ifdef ZP_USE_PRINT
zp_int zp_printf( const zp_char* text, ... )
{
    zp_char buff[ ZP_PRINT_BUFFER_SIZE ];
    zp_int r;

    va_list vl;
    va_start( vl, text );
#ifdef ZP_USE_SAFE_FUNCTIONS
    vsprintf_s( buff, text, vl );
    r = printf_s( buff );
#else // !ZP_USE_SAFE_FUNCTIONS
    vsprintf( buff, text, vl );
    r = printf( buff );
#endif // ZP_USE_SAFE_FUNCTIONS
    va_end( vl );

#if defined( ZP_WINDOWS ) && defined( ZP_DEBUG )
    OutputDebugString( buff );
#endif // !ZP_WINDOWS && !ZP_DEBUG

    return r;
}

zp_int zp_printfln( const zp_char* text, ... )
{
    zp_char buff[ ZP_PRINT_BUFFER_SIZE ];
    zp_int r;

    va_list vl;
    va_start( vl, text );
#ifdef ZP_USE_SAFE_FUNCTIONS
    zp_int end = vsprintf_s( buff, text, vl );
    sprintf_s( buff + end, sizeof( buff ) - end, "\n" );
    r = printf_s( buff );
#else // !ZP_USE_SAFE_FUNCTIONS
    zp_int end = vsprintf( buff, text, vl );
    sprintf( buff + end, "\n" );
    r = printf( buff );
#endif // ZP_USE_SAFE_FUNCTIONS
    va_end( vl );

#if defined( ZP_WINDOWS ) && defined( ZP_DEBUG )
    OutputDebugString( buff );
#endif // !ZP_WINDOWS && !ZP_DEBUG

    return r;
}
#endif // !ZP_USE_PRINT

zp_int zp_snprintf( zp_char* dest, zp_size_t destSize, zp_size_t maxCount, const zp_char* format, ... )
{
    va_list vl;
    zp_int r;

    va_start( vl, format );
#ifdef ZP_USE_SAFE_FUNCTIONS
    r = vsnprintf_s( dest, destSize, maxCount, format, vl );
#else
    r = vsnprintf( dest, destSize, format, vl );
#endif
    va_end( vl );

    return r;
}