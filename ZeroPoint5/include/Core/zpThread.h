#pragma once
#ifndef ZP_THREAD_H
#define ZP_THREAD_H

enum zpThreadPriority : zp_int
{
    ZP_THREAD_PRIORITY_IDLE = -15,
    ZP_THREAD_PRIORITY_LOWEST = -2,
    ZP_THREAD_PRIORITY_BELOW_NORMAL = -1,
    ZP_THREAD_PRIORITY_NORMAL = 0,
    ZP_THREAD_PRIORITY_ABOVE_NORMAL = 1,
    ZP_THREAD_PRIORITY_HIGHEST = 2,
    ZP_THREAD_PRIORITY_TIME_CRITICAL = 15,
};

typedef zp_dword zp_thread_id_t;
typedef void ( *zpThreadFunc )( void* ptr );

typedef struct
{
    zp_handle handle;
    zp_thread_id_t id;
    zpThreadFunc func;
    void* ptr;
} zp_thread_t;

namespace zpThread
{
    zp_bool Create( zp_thread_t* thread );
    void Destroy( zp_thread_t* thread );

    void Join( zp_thread_t* thread );

    void Start( zp_thread_t* thread, zpThreadFunc func, void* ptr );
    void SetPriority( zp_thread_t* thread, zpThreadPriority priority );

    zp_thread_id_t GetId( zp_thread_t* thread );

    zp_thread_id_t CurrentThreadId();

    void YieldCurrentThread();

    zp_size_t GetNumProcessors();
};

#endif // !ZP_THREAD_H
