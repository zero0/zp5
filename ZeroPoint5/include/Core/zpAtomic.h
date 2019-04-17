#pragma once
#ifndef ZP_ATOMIC_H
#define ZP_ATOMIC_H

#define ZP_COMPILER_BARRIER zpAtomic::ReadWriteBarrier()

namespace zpAtomic
{
    zp_int Add32( volatile zp_int* addend, zp_int value );

    zp_long Add64( volatile zp_long* addend, zp_long value );

    zp_size_t AddSize( volatile zp_size_t* addend, zp_size_t value );

    zp_int Increment32( volatile zp_int* addend );

    zp_long Increment64( volatile zp_long* addend );

    zp_int Decrement32( volatile zp_int* addend );

    zp_long Decrement64( volatile zp_long* addend );

    zp_int Exchange32( volatile zp_int* target, zp_int value );

    zp_long Exchange64( volatile zp_long* target, zp_long value );

    zp_size_t ExchangeSize( volatile zp_size_t* target, zp_size_t value );

    void* ExchangePtr( volatile void** target, void* value );

    zp_int CompareExchange32( volatile zp_int* target, zp_int exchange, zp_int comparent );

    zp_long CompareExchange64( volatile zp_long* target, zp_long exchange, zp_long comparent );

    zp_size_t CompareExchangeSize( volatile zp_size_t* target, zp_size_t exchange, zp_size_t comparent );

    void MemBarrier();

    void ReadWriteBarrier();
};

#endif // !ZP_ATOMIC_H
