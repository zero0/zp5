#include "Core/zpCore.h"

#include <stdint.h>

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // !ZP_WINDOWS

zp_int zpAtomic::Add32( volatile zp_int* added, zp_int value )
{
    return InterlockedAdd( (volatile LONG*)added, value );
}

zp_long zpAtomic::Add64( volatile zp_long* added, zp_long value )
{
    return InterlockedAdd64( added, value );
}

zp_size_t zpAtomic::AddSize( volatile zp_size_t* added, zp_size_t value )
{
#if ZP_64BIT
    return InterlockedAdd64( (volatile zp_long*)added, value );
#else // !ZP_64BIT
    return InterlockedAdd( (volatile zp_int*)added, value );
#endif // ZP_64BIT
}

zp_int zpAtomic::Increment32( volatile zp_int* addend )
{
    return InterlockedIncrement( (volatile LONG*)addend );
}

zp_long zpAtomic::Increment64( volatile zp_long* addend )
{
    return InterlockedIncrement64( addend );
}

zp_int zpAtomic::Decrement32( volatile zp_int* addend )
{
    return InterlockedDecrement( (volatile LONG*)addend );
}

zp_long zpAtomic::Decrement64( volatile zp_long* addend )
{
    return InterlockedDecrement64( addend );
}

zp_int zpAtomic::Exchange32( volatile zp_int* target, zp_int value )
{
    return InterlockedExchange( (volatile LONG*)target, value );
}

zp_long zpAtomic::Exchange64( volatile zp_long* target, zp_long value )
{
    return InterlockedExchange64( target, value );
}

zp_size_t zpAtomic::ExchangeSize( volatile zp_size_t* target, zp_size_t value )
{
#if ZP_64BIT
    return InterlockedExchange64( (volatile zp_long*)target, value );
#else // !ZP_64BIT
    return InterlockedExchange( (volatile zp_int*)added, value );
#endif // ZP_64BIT
}

void* zpAtomic::ExchangePtr( volatile void** target, void* value )
{
    return InterlockedExchangePointer( (volatile PVOID*)target, value );
}

zp_int zpAtomic::CompareExchange32( volatile zp_int* target, zp_int exchange, zp_int comparent )
{
    return InterlockedCompareExchange( (volatile LONG*)target, exchange, comparent );
}

zp_long zpAtomic::CompareExchange64( volatile zp_long* target, zp_long exchange, zp_long comparent )
{
    return InterlockedCompareExchange64( target, exchange, comparent );
}

zp_size_t zpAtomic::CompareExchangeSize( volatile zp_size_t* target, zp_size_t exchange, zp_size_t comparent )
{
#if ZP_64BIT
    return InterlockedCompareExchange64( (volatile zp_long*)target, exchange, comparent );
#else // !ZP_64BIT
    return InterlockedCompareExchange( (volatile zp_int*)target, exchange, comparent );
#endif // ZP_64BIT
}

void zpAtomic::MemBarrier()
{
    MemoryBarrier();
}

void zpAtomic::ReadWriteBarrier()
{
    _ReadWriteBarrier();
}
