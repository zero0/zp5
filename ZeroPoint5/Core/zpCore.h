#pragma once
#ifndef ZP_CORE_H
#define ZP_CORE_H

//
// Defines
//

#if defined(DEBUG) || defined(_DEBUG)
#define ZP_DEBUG                    1
#endif

#if defined(_WIN64) || defined(_M_X64) || defined(_M_AMD64) || defined(_M_IA64)
#define ZP_WIN_64                   1
#endif
#if defined(_WIN32)
#define ZP_WIN_32                   1
#endif

#define ZP_MALLOC_ALIGNMENT         16
#define ZP_FORECE_32BIT             0x7FFFFFFF

#define ZP_STDCALL                  __stdcall
#define ZP_FASTCALL                 __fastcall

#if ZP_WIN_32
#define ZP_VECTORCALL               __fastcall
#elif ZP_WIN_64
#define ZP_VECTORCALL               __vectorcall
#endif

#define ZP_INLINE                   inline
#define ZP_FORCE_INLINE             __forceinline
#define ZP_NO_VTABLE                __declspec( novtable )
#define ZP_ALIGN(x)                 __declspec( align( x ) )
#define ZP_ALIGN16                  ZP_ALIGN( 16 )

#define ZP_PURE_INTERFACE           class ZP_NO_VTABLE
#define ZP_ABSTRACT_CLASS           class ZP_NO_VTABLE

#define ZP_UNUSED( v )              (void)v

#if __cplusplus
#define ZP_NULL    nullptr
#else
#define ZP_NULL    0
#endif

#if ZP_WIN_32
#define ZP_W64    __w64
#else
#define ZP_W64
#endif

#include "zpConfig.h"

//
// Types
//

typedef size_t zp_size_t;

typedef unsigned __int8  zp_byte;
typedef unsigned __int16 zp_ushort;
typedef unsigned __int32 zp_uint;
typedef unsigned __int64 zp_ulong;

typedef signed __int8   zp_sbyte;
typedef signed __int16  zp_short;
typedef signed __int32  zp_int;
typedef signed __int64  zp_long;

typedef float   zp_float;
typedef double  zp_double;

typedef bool zp_bool;
typedef char zp_char;

typedef void* zp_handle;

typedef zp_ushort   zp_word;
typedef zp_uint     zp_dword;
typedef zp_ulong    zp_qword;

typedef zp_uint     zp_hash32;
typedef zp_ulong    zp_hash64;

//
// Functions
//

void* zp_malloc( zp_size_t size );
void zp_free( void* ptr );

#ifdef ZP_USE_ASSERTIONS
#define ZP_ASSERT( test, msg, ... )         if( !(test) ) { zp_assert( __FILE__, __LINE__, msg, __VA_ARGS__ ); } (void)0
#define ZP_ASSERT_WARN( test, msg, ... )    if( !(test) ) { zp_assert_warning( __FILE__, __LINE__, msg, __VA_ARGS__ ); } (void)0
#else
#define ZP_ASSERT( test, msg, ... )         (void)0
#define ZP_ASSERT_WARN( test, msg, ... )    (void)0
#endif

#ifdef ZP_USE_ASSERTIONS
void zp_assert( const zp_char* file, zp_int line, const zp_char* msg, ... );
void zp_assert_warning( const zp_char* file, zp_int line, const zp_char* msg, ... );
#endif

#ifdef ZP_USE_PRINT
void zp_printf( const zp_char* text, ... );
void zp_printfln( const zp_char* text, ... );
#else
#define zp_printf( text, ... ) (void)0
#define zp_printfln( text, ... ) (void)0
#endif

#include "zpBlockAllocator.h"

#endif // !ZP_CORE_H

