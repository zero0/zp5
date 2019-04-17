#include "Core/zpCore.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // !ZP_WINDOWS

static DWORD ThreadProc( void* ptr )
{
    zp_thread_t* thread = static_cast<zp_thread_t*>( ptr );
    
    void* data = thread->ptr;
    zpThreadFunc func = thread->func;

    thread->ptr = ZP_NULL;
    thread->func = ZP_NULL;

    if( func )
    {
        func( data );
    }

    return 0;
}

zp_bool zpThread::Create( zp_thread_t* thread )
{
    thread->func = ZP_NULL;
    thread->ptr = ZP_NULL;
    thread->handle = CreateThread( ZP_NULL, 0, ThreadProc, thread, CREATE_SUSPENDED, &thread->id );

    return thread->handle != ZP_NULL;
}

void zpThread::Destroy( zp_thread_t* thread )
{
    CloseHandle( thread->handle );

    thread->handle = ZP_NULL;
    thread->func = ZP_NULL;
    thread->ptr = ZP_NULL;
    thread->id = 0;
}

void zpThread::Start( zp_thread_t* thread, zpThreadFunc func, void* ptr )
{
    thread->func = func;
    thread->ptr = ptr;

    ResumeThread( thread->handle );
}

void zpThread::Join( zp_thread_t* thread )
{
    WaitForSingleObject( thread->handle, INFINITE );
}

void zpThread::SetPriority( zp_thread_t* thread, zpThreadPriority priority )
{
    SetThreadPriority( thread->handle, priority );
}

zp_thread_id_t zpThread::GetId( zp_thread_t* thread )
{
    return thread->id;
}

zp_thread_id_t zpThread::CurrentThreadId()
{
    return GetCurrentThreadId();
}

void zpThread::YieldCurrentThread()
{
    SwitchToThread();
}

zp_size_t zpThread::GetNumProcessors()
{
#if ZP_WINDOWS
    SYSTEM_INFO info;
    GetSystemInfo( &info );
    return info.dwNumberOfProcessors;
#else
    return 0;
#endif
}
