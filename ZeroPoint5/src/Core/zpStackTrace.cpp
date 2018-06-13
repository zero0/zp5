#include "Core/zpCore.h"

#if ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dbghelp.h>
#pragma comment( lib, "dbghelp.lib" )
#endif

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */

#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

zpStackTrace::zpStackTrace()
    : m_count( 0 )
    , m_hash( 0 )
{
#if ZP_WINDOWS
    m_count = CaptureStackBackTrace( 0, ZP_STACK_TRACE_MAX_TRACES, m_stackTrace, (PDWORD)&m_hash );
#endif
}

zpStackTrace::~zpStackTrace()
{
}

void zpStackTrace::print() const
{
#if ZP_WINDOWS
    HANDLE process;
    process = GetCurrentProcess();

    const zp_uint maxNameLen = 255;

    zp_byte symbolBuff[ sizeof( SYMBOL_INFO ) + maxNameLen * sizeof( zp_char ) ];
    zp_zero_memory_array( symbolBuff );

    PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbolBuff;
    symbol->MaxNameLen = maxNameLen;
    symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

    for( zp_uint i = 0; i < m_count; ++i )
    {
        void* stack = m_stackTrace[ i ];

        SymFromAddr( process, (DWORD64)stack, 0, symbol );

        zp_printfln( GREEN "%2i: 0x%0X - %-64s" RESET, m_count - i - 1, stack, symbol->Name );
    }
#endif
}

zp_uint zpStackTrace::size() const
{
    return m_count;
}

zp_hash32 zpStackTrace::hash() const
{
    return m_hash;
}

void zpStackTrace::Initialize()
{
#if ZP_WINDOWS
    HANDLE process;
    zp_bool ok;

    process = GetCurrentProcess();

    SymSetOptions( SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES );
    ok = !!SymInitialize( process, ZP_NULL, true );
    ZP_ASSERT( ok, "Failed to initilize Sym" );
#endif
}

void zpStackTrace::Shutdown()
{
#if ZP_WINDOWS
    HANDLE process;
    zp_bool ok;

    process = GetCurrentProcess();

    ok = !!SymCleanup( process );
    ZP_ASSERT( ok, "Failed to shutdown Sym" );
#endif
}

void zpStackTrace::Dump()
{
    zpStackTrace trace;
    trace.print();
}