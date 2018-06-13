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
#define ZP_64BIT                    1
#elif defined(_WIN32)
#define ZP_WIN_32                   1
#endif

#if defined( ZP_WIN_32 ) || defined( ZP_WIN_64 )
#define ZP_WINDOWS                  1
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
#define ZP_GLOBAL_CONST             extern const __declspec( selectany )

#define ZP_CLASS                    class
#define ZP_PURE_INTERFACE           class ZP_NO_VTABLE
#define ZP_ABSTRACT_CLASS           class ZP_NO_VTABLE
#define ZP_SEALED_CLASS( x )        class x final
#define ZP_SEALED_STRUCT( x )       struct x final

#define ZP_NON_COPYABLE( t )        private: t( const t& ){} t& operator=( const t& ){}

#define ZP_UNUSED( v )              (void)v

#define ZP_MEMORY_KB( b )           static_cast<zp_size_t>( (b) * 1024 )
#define ZP_MEMORY_MB( b )           static_cast<zp_size_t>( ZP_MEMORY_KB( b ) * 1024 )
#define ZP_MEMORY_GB( b )           static_cast<zp_size_t>( ZP_MEMORY_MB( b ) * 1024 )

#define ZP_CONCAT_( a, b )          a##b
#define ZP_CONCAT( a, b )           ZP_CONCAT_( a, b )
#define ZP_STATIC_ASSERT( e )       ; enum { ZP_CONCAT( static_assert_, __LINE__ ) = 1 / static_cast<int>( !!(e) ) }

#define ZP_ARRAY_SIZE( a )          ( sizeof( a ) / sizeof( a[ 0 ] ) )

#define ZP_CONSTEXPR                constexpr

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

typedef unsigned __int64 zp_time_t;

#if ZP_64BIT
typedef unsigned __int64 zp_size_t;
typedef signed __int64 zp_ptrdiff_t;
#else
typedef unsigned __int32 zp_size_t;
typedef signed __int32 zp_ptrdiff_t;
#endif

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

typedef zp_uint     zp_flag32;
typedef zp_ulong    zp_flag64;

//
// Functions
//

void* zp_malloc( zp_size_t size );
void zp_free( void* ptr );

void zp_sleep( zp_int milliseconds );

void* zp_memcpy( void* dest, zp_size_t destSize, const void* src, zp_size_t size );
void* zp_memset( void* dest, zp_int value, zp_size_t size );
zp_int zp_memcmp( const void* ptr1, const void* ptr2, zp_size_t size );

zp_char* zp_strcpy( zp_char* destString, zp_size_t numElements, const zp_char* srcString );
zp_char* zp_strncpy( zp_char* destString, zp_size_t numElements, const zp_char* srcString, zp_size_t maxCount );
zp_size_t zp_strlen( const zp_char* srcString );
zp_int zp_strcmp( const zp_char* str1, const zp_char* str2 );
zp_int zp_stricmp( const zp_char* str1, const zp_char* str2 );

//
// Template function
//

template<typename T>
ZP_FORCE_INLINE void zp_zero_memory( T* ptr )
{
    zp_memset( ptr, 0, sizeof( T ) );
}
template<typename T>
ZP_FORCE_INLINE void zp_zero_memory_array( T* ptr, zp_size_t count )
{
    zp_memset( ptr, 0, count * sizeof( T ) );
}
template<typename T, zp_size_t Size>
ZP_FORCE_INLINE void zp_zero_memory_array( T( &arr )[ Size ] )
{
    zp_memset( arr, 0, Size * sizeof( T ) );
}

template<typename T>
ZP_FORCE_INLINE void zp_swap( T& a, T& b );

template<typename T>
ZP_FORCE_INLINE void zp_move_swap( T& a, T& b );

template<typename T>
ZP_FORCE_INLINE void zp_move_swap( T&& a, T&& b );

template<typename T>
ZP_FORCE_INLINE ZP_CONSTEXPR T&& zp_forward( T& a );

template<typename T>
ZP_FORCE_INLINE ZP_CONSTEXPR T&& zp_forward( T&& a );

template<typename T>
ZP_FORCE_INLINE ZP_CONSTEXPR T&& zp_move( T&& a );

template<typename T, typename Cmp>
void zp_qsort( T* begin, T* end, Cmp cmp );

ZP_FORCE_INLINE ZP_CONSTEXPR zp_flag64 zp_flag_mark( zp_flag64 flag, zp_ulong index )
{
    return flag | ( static_cast<zp_ulong>( 1 ) << index );
}

ZP_FORCE_INLINE ZP_CONSTEXPR zp_flag64 zp_flag_clear( zp_flag64 flag, zp_ulong index )
{
    return flag & ~( static_cast<zp_ulong>( 1 ) << index );
}

ZP_FORCE_INLINE ZP_CONSTEXPR zp_flag64 zp_flag_toggle( zp_flag64 flag, zp_ulong index )
{
    return flag ^ ( static_cast<zp_ulong>( 1 ) << index );
}

ZP_FORCE_INLINE ZP_CONSTEXPR zp_flag64 zp_flag_set( zp_flag64 flag, zp_ulong index, zp_bool set )
{
    return ( ( set ? static_cast<zp_ulong>( -1 ) : 0 ) ^ flag ) & ( static_cast<zp_ulong>( 1 ) << index );
}

ZP_FORCE_INLINE ZP_CONSTEXPR zp_bool zp_flag_is_set( zp_flag64 flag, zp_ulong index )
{
    return ( flag & ( static_cast<zp_ulong>( 1 ) << index ) ) != 0;
}

//
// Macros
//

#ifdef ZP_USE_ASSERTIONS
#define ZP_ASSERT( test, msg, ... )         if( !(test) ) { zp_assert( __FILE__, __LINE__, msg, __VA_ARGS__ ); }         static_cast<void>( 0 )
#define ZP_ASSERT_WARN( test, msg, ... )    if( !(test) ) { zp_assert_warning( __FILE__, __LINE__, msg, __VA_ARGS__ ); } static_cast<void>( 0 )
#define ZP_INVALID_CODE_PATH()              zp_assert( __FILE__, __LINE__, "Invalid Code Path" )
#else
#define ZP_ASSERT( test, msg, ... )         static_cast<void>( 0 )
#define ZP_ASSERT_WARN( test, msg, ... )    static_cast<void>( 0 )
#define ZP_INVALID_CODE_PATH()              static_cast<void>( 0 )
#endif

#ifdef ZP_USE_ASSERTIONS
void zp_assert( const zp_char* file, zp_int line, const zp_char* msg, ... );
void zp_assert_warning( const zp_char* file, zp_int line, const zp_char* msg, ... );
#endif

#ifdef ZP_USE_PRINT
zp_int zp_printf( const zp_char* text, ... );
zp_int zp_printfln( const zp_char* text, ... );
#else
#define zp_printf( text, ... )      static_cast<void>( 0 )
#define zp_printfln( text, ... )    static_cast<void>( 0 )
#endif

zp_int zp_snprintf( zp_char* dest, zp_size_t destSize, zp_size_t maxCount, const zp_char* format, ... );

//
// Includes
//

#include "zpMath.h"
#include "zpTLSF.h"
#include "zpLZF.h"
#include "zpMemoryAllocator.h"
#include "zpMemoryPolicy.h"
#include "zpMemoryStorage.h"
#include "zpTime.h"
#include "zpString.h"
#include "zpVector.h"
#include "zpDataBuffer.h"
#include "zpFile.h"
#include "zpArchive.h"

#include "zpProfiler.h"
#include "zpStackTrace.h"

#include "zpCore.inl"

#if ZP_DEBUG
extern zpIMemoryAllocator* g_globalDebugAllocator;
#endif

extern zpIMemoryAllocator* g_globalAllocator;

//
//
//

struct zpDefaultGlobalAllocator
{
    ZP_FORCE_INLINE void* allocate( zp_size_t size )
    {
        void* ptr = g_globalAllocator->allocate( size );
        return ptr;
    }

    ZP_FORCE_INLINE void free( const void* ptr )
    {
        g_globalAllocator->free( ptr );
    }
};

struct zpDefaultGlobalArrayAllocator
{
    ZP_FORCE_INLINE void* allocate( zp_size_t size, zp_size_t count )
    {
        void* ptr = g_globalAllocator->allocate( size * count );
        return ptr;
    }

    ZP_FORCE_INLINE void free( const void* ptr )
    {
        g_globalAllocator->free( ptr );
    }
};

template< typename T >
struct zpDefaultGlobalTemplateAllocator
{
    ZP_FORCE_INLINE void* allocate()
    {
        void* ptr = g_globalAllocator->allocate( sizeof( T ) );
        return ptr;
    }

    ZP_FORCE_INLINE void free( const void* ptr )
    {
        g_globalAllocator->free( ptr );
    }
};

template< typename T >
struct zpDefaultGlobalTemplateArrayAllocator
{
    ZP_FORCE_INLINE void* allocate( zp_size_t count )
    {
        void* ptr = g_globalAllocator->allocate( sizeof( T ) * count );
        return ptr;
    }

    ZP_FORCE_INLINE void free( const void* ptr )
    {
        g_globalAllocator->free( ptr );
    }
};

#endif // !ZP_CORE_H

