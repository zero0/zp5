#include "zpCore.h"
#include <memory>
#include <stdarg.h>

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
#else
    vsnprintf( message, sizeof( message ), msg, vl );
#endif
    va_end( vl );

#ifdef ZP_USE_SAFE_FUNCTIONS
    _snprintf_s( text, sizeof( text ), sizeof( text ), "%s\n\nDebug Break (Abort)  Continue (Retry)  Ignore", message );
    _snprintf_s( title, sizeof( title ), sizeof( title ), "ZeroPoint Assert Failed at %s:%d", file, line );
#else
    snprintf( text, sizeof( text ), "%s\n\nDebug Break (Abort)  Continue (Retry)  Ignore", message );
    snprintf( title, sizeof( title ), "ZeroPoint Assert Failed at %s:%d", file, line );
#endif

    //zp_int result = MessageBox( ZP_NULL, text, title, MB_ABORTRETRYIGNORE | MB_ICONERROR );
    //switch( result )
    //{
    //    case IDOK:
    //    case IDYES:
    //    case IDABORT:
    //        __debugbreak();
    //        break;
    //    case IDNO:
    //    case IDRETRY:
    //        // continue
    //        break;
    //    case IDCANCEL:
    //    case IDIGNORE:
    //        // ignore
    //        break;
    //}
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
#else
    vsnprintf( message, sizeof( message ), msg, vl );
#endif
    va_end( vl );

#ifdef ZP_USE_SAFE_FUNCTIONS
    _snprintf_s( text, sizeof( text ), sizeof( text ), "%s\n\nDebug Break (Retry)  Continue (Cancel)", message );
    _snprintf_s( title, sizeof( title ), sizeof( title ), "ZeroPoint Assert Warning at %s:%d", file, line );
#else
    snprintf( text, sizeof( text ), "%s\n\nDebug Break (Retry)  Continue (Cancel)", message );
    snprintf( title, sizeof( title ), "ZeroPoint Assert Warning at %s:%d", file, line );
#endif

    //zp_int result = MessageBox( ZP_NULL, text, title, MB_RETRYCANCEL | MB_ICONWARNING );
    //switch( result )
    //{
    //    case IDRETRY:
    //        __debugbreak();
    //        break;
    //    case IDCANCEL:
    //        // ignore
    //        break;
    //}
}
#endif // !ZP_USE_ASSERTIONS

#ifdef ZP_USE_PRINT
void zp_printf( const zp_char* text, ... )
{
    zp_char buff[ ZP_PRINT_BUFFER_SIZE ];

    va_list vl;
    va_start( vl, text );
#ifdef ZP_USE_SAFE_FUNCTIONS
    vsprintf_s( buff, text, vl );
    printf_s( buff );
#else
    vsprintf( buff, text, vl );
    printf( buff );
#endif
    va_end( vl );

    //OutputDebugString( buff );
}
void zp_printfln( const zp_char* text, ... )
{
    zp_char buff[ ZP_PRINT_BUFFER_SIZE ];

    va_list vl;
    va_start( vl, text );
#ifdef ZP_USE_SAFE_FUNCTIONS
    zp_int end = vsprintf_s( buff, text, vl );
    sprintf_s( buff + end, sizeof( buff ) - end, "\n" );
    printf_s( buff );
#else
    zp_int end = vsprintf( buff, text, vl );
    sprintf( buff + end, "\n" );
    printf( buff );
#endif
    va_end( vl );

    //OutputDebugString( buff );
}
#endif // !ZP_USE_PRINT
