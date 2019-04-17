#include "Core/zpCore.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // !ZP_WINDOWS

enum
{
    ZP_MAX_JOB_COUNT = 4096,
    ZP_MAX_JOB_COUNT_MASK = ZP_MAX_JOB_COUNT - 1
};

//
//
//

class zpJobQueue
{
public:
    zpJobQueue();
    ~zpJobQueue();

    void push( zpJob* job );
    zpJob* pop();
    zpJob* steal();

    zp_bool isEmpty() const;
    zp_size_t count() const;

private:
    zp_size_t m_bottom;
    zp_size_t m_top;
    zpJob* m_queue[ ZP_MAX_JOB_COUNT ];
};

zpJobQueue::zpJobQueue()
    : m_bottom( 0 )
    , m_top( 0 )
{
}

zpJobQueue::~zpJobQueue()
{
}

void zpJobQueue::push( zpJob* job )
{
    const zp_size_t b = m_bottom;
    m_queue[ b & ZP_MAX_JOB_COUNT_MASK ] = job;

    //ZP_COMPILER_BARRIER;
    MemoryFence();

    zpAtomic::ExchangeSize( &m_bottom, b + 1 );
}

zpJob* zpJobQueue::pop()
{
    const zp_size_t b = m_bottom - 1;

    zpAtomic::ExchangeSize( &m_bottom, b );

    const zp_size_t t = m_top;
    if( t <= b )
    {
        zpJob* job = m_queue[ b & ZP_MAX_JOB_COUNT_MASK ];
        if( t != b )
        {
            return job;
        }

        if( zpAtomic::CompareExchangeSize( &m_top, t + 1, t ) != t )
        {
            job = ZP_NULL;
        }

        m_bottom = t + 1;
        return job;

    }
    else
    {
        m_bottom = t;
        return ZP_NULL;
    }
}

zpJob* zpJobQueue::steal()
{
    const zp_size_t t = m_top;

    //ZP_COMPILER_BARRIER;
    MemoryFence();

    const zp_size_t b = m_bottom;
    if( t < b )
    {
        zpJob* job = m_queue[ t & ZP_MAX_JOB_COUNT_MASK ];

        if( zpAtomic::CompareExchangeSize( &m_top, t + 1, t ) != t )
        {
            return ZP_NULL;
        }

        return job;
    }
    else
    {
        return ZP_NULL;
    }
}

zp_bool zpJobQueue::isEmpty() const
{
    return m_bottom == m_top;
}

zp_size_t zpJobQueue::count() const
{
    return m_bottom - m_top;
}

//
//
//

thread_local static zpJob g_jobAllocator[ ZP_MAX_JOB_COUNT ];
thread_local static zp_size_t g_allocatedJobs = 0;
thread_local static zpJobQueue* g_localJobQueue = ZP_NULL;
thread_local static zp_bool g_isRunning = false;

static zpJob* AllocateJob()
{
    const zp_size_t index = g_allocatedJobs++;
    return &g_jobAllocator[ index & ZP_MAX_JOB_COUNT_MASK ];
}

static zpJobQueue* GetLocalThreadJobQueue()
{
    return g_localJobQueue;
}

static zp_size_t g_workerThreadCount;
static zp_size_t g_workerQueueCount;
static zp_size_t g_nextWorkerThreadCount;
static zp_thread_t* g_workerThreads;
static zpJobQueue* g_jobQueues;

static zpJobQueue* GetRandomJobQueue()
{
    const zp_size_t index = g_nextWorkerThreadCount++ % g_workerQueueCount;
    return &g_jobQueues[ index ];
}

//
//
//

ZP_FORCE_INLINE zp_bool IsJobComplete( const zpJob* job )
{
    return job->unfinishedJobs <= 0;
}

static void RunJob( zpJob* job )
{
    zpJobQueue* queue = GetLocalThreadJobQueue();
    queue->push( job );
}

static void FinishJob( zpJob* job )
{
    const zp_size_t unfinishedJobs = zpAtomic::Decrement32( &job->unfinishedJobs );
    if( unfinishedJobs == 0 )
    {
        if( job->parent )
        {
            FinishJob( job->parent );
        }

        if( job->next )
        {
            RunJob( job->next );
        }
    }
}

static void ExecuteJob( zpJob* job )
{
    job->func( job, job->padding );
    FinishJob( job );
}

static zpJob* GetJob()
{
    zpJobQueue* queue = GetLocalThreadJobQueue();

    zpJob* job = ZP_NULL;

    if( queue->isEmpty() )
    {
        zpJobQueue* stealQueue = GetRandomJobQueue();
        if( queue == stealQueue || stealQueue->isEmpty() )
        {
            zpThread::YieldCurrentThread();
        }
        else
        {
            job = stealQueue->steal();
        }
    }
    else
    {
        job = queue->pop();
    }

    return job;
}

static void WaitForJob( const zpJob* job )
{
    while( !IsJobComplete( job ) )
    {
        zpJob* nextJob = GetJob();
        if( nextJob )
        {
            ExecuteJob( nextJob );
        }
    }
}

//
//
//

static void WorkerThreadExection( void* data )
{
    g_localJobQueue = static_cast<zpJobQueue*>( data );
    g_isRunning = true;

    while( g_isRunning )
    {
        zpJob* job = GetJob();
        if( job )
        {
            ExecuteJob( job );
        }
    }

    g_localJobQueue = ZP_NULL;
}

static void ShutdownThread( zpJob* job, const void* data )
{
    g_isRunning = false;
}

//
//
//

void zpJobSystem::Initialize( zpIMemoryAllocator* allocator, zp_size_t numWorkerThreads )
{
    g_workerThreadCount = numWorkerThreads;
    g_workerQueueCount = numWorkerThreads + 1;

    g_workerThreads = static_cast<zp_thread_t*>( allocator->allocate( sizeof( zp_thread_t ) * g_workerThreadCount ) );
    g_jobQueues = static_cast<zpJobQueue*>( allocator->allocate( sizeof( zpJobQueue ) * g_workerQueueCount ) );

    zp_zero_memory_array( g_workerThreads, g_workerThreadCount );
    zp_zero_memory_array( g_jobQueues, g_workerQueueCount );

    for( zp_size_t i = 0; i < g_workerThreadCount; ++i )
    {
        zpThread::Create( &g_workerThreads[ i ] );
        zpThread::Start( &g_workerThreads[ i ], WorkerThreadExection, &g_jobQueues[ i ] );
    }

    // main thread job queue
    g_localJobQueue = &g_jobQueues[ g_workerThreadCount ];
}

void zpJobSystem::Shutdown( zpIMemoryAllocator* allocator )
{
    for( zp_size_t i = 0; i < g_workerThreadCount; ++i )
    {
        zpJob* shutdownJob = Create( ShutdownThread );
        g_jobQueues[ i ].push( shutdownJob );
    }

    zpThread::Join( g_workerThreads );
    zpThread::Destroy( g_workerThreads );

    allocator->free( g_workerThreads );
    g_workerThreads = ZP_NULL;

    allocator->free( g_jobQueues );
    g_jobQueues = ZP_NULL;

    g_workerThreadCount = 0;
}

zpJob* zpJobSystem::Create( zpJobFunction func )
{
    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = ZP_NULL;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    return job;
}

zpJob* zpJobSystem::Create( zpJobFunction func, zpJob* dependency )
{
    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = ZP_NULL;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    dependency->next = job;

    return job;
}

zpJob* zpJobSystem::Create( zpJobFunction func, const void* data, zp_size_t size )
{
    ZP_ASSERT( size < ZP_JOB_PADDING_SIZE, "" );

    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = ZP_NULL;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    zp_memcpy( job->padding, ZP_JOB_PADDING_SIZE, data, size );

    return job;
}

zpJob* zpJobSystem::Create( zpJobFunction func, const void* data, zp_size_t size, zpJob* dependency )
{
    ZP_ASSERT( size < ZP_JOB_PADDING_SIZE, "" );

    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = ZP_NULL;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    zp_memcpy( job->padding, ZP_JOB_PADDING_SIZE, data, size );

    dependency->next = job;

    return job;
}

zpJob* zpJobSystem::CreateChild( zpJob* parent, zpJobFunction func )
{
    zpAtomic::Increment32( &parent->unfinishedJobs );

    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = parent;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    return job;
}

zpJob* zpJobSystem::CreateChild( zpJob* parent, zpJobFunction func, zpJob* dependency )
{
    zpAtomic::Increment32( &parent->unfinishedJobs );

    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = parent;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    dependency->next = job;

    return job;
}

zpJob* zpJobSystem::CreateChild( zpJob* parent, zpJobFunction func, const void* data, zp_size_t size )
{
    ZP_ASSERT( size < ZP_JOB_PADDING_SIZE, "" );

    zpAtomic::Increment32( &parent->unfinishedJobs );

    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = parent;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    zp_memcpy( job->padding, ZP_JOB_PADDING_SIZE, data, size );

    return job;
}

zpJob* zpJobSystem::CreateChild( zpJob* parent, zpJobFunction func, const void* data, zp_size_t size, zpJob* dependency )
{
    ZP_ASSERT( size < ZP_JOB_PADDING_SIZE, "" );

    zpAtomic::Increment32( &parent->unfinishedJobs );

    zpJob* job = AllocateJob();
    job->func = func;
    job->parent = parent;
    job->next = ZP_NULL;
    job->unfinishedJobs = 1;

    zp_memcpy( job->padding, ZP_JOB_PADDING_SIZE, data, size );

    dependency->next = job;

    return job;
}

void zpJobSystem::Run( zpJob* job )
{
    RunJob( job );
}

void zpJobSystem::Wait( const zpJob* job )
{
    WaitForJob( job );
}
