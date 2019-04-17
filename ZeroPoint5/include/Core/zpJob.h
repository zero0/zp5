#pragma once
#ifndef ZP_JOB_H
#define ZP_JOB_H

struct zpJob;
typedef void ( *zpJobFunction )( zpJob* job, const void* data );

enum
{
    ZP_JOB_SIZE = ZP_MEMORY_KB( 4 ),
    ZP_JOB_PADDING_SIZE = ZP_JOB_SIZE - ( sizeof( zpJobFunction ) + sizeof( zpJob* ) + sizeof( zpJob* ) + sizeof( zp_int ) )
};

class zpJobHandle
{
public:
    zp_bool isDone() const;
    void complete() const;

private:
    zpJob* m_job;
};

struct zpJob
{
    zpJobFunction func;
    zpJob* parent;
    zpJob* next;
    zp_int unfinishedJobs;
    zp_char padding[ ZP_JOB_PADDING_SIZE ];
};
ZP_STATIC_ASSERT( sizeof( zpJob ) == ZP_JOB_SIZE );

namespace zpJobSystem
{
    void Initialize( zpIMemoryAllocator* allocator, zp_size_t numWorkerThreads );
    void Shutdown( zpIMemoryAllocator* allocator );

    zpJob* Create( zpJobFunction func );
    zpJob* Create( zpJobFunction func, zpJob* dependency );
    zpJob* Create( zpJobFunction func, const void* data, zp_size_t size );
    zpJob* Create( zpJobFunction func, const void* data, zp_size_t size, zpJob* dependency );

    zpJob* CreateChild( zpJob* parent, zpJobFunction func );
    zpJob* CreateChild( zpJob* parent, zpJobFunction func, zpJob* dependency );
    zpJob* CreateChild( zpJob* parent, zpJobFunction func, const void* data, zp_size_t size );
    zpJob* CreateChild( zpJob* parent, zpJobFunction func, const void* data, zp_size_t size, zpJob* dependency );

    template<typename T>
    zpJob* Create( zpJobFunction func, const T& data )
    {
        return Create( func, &data, sizeof( T ) );
    }

    template<typename T>
    zpJob* Create( zpJobFunction func, const T& data, zpJob* dependency )
    {
        return Create( func, &data, sizeof( T ), dependency );
    }

    template<typename T>
    zpJob* CreateChild( zpJob* parent, zpJobFunction func, const T& data )
    {
        return CreateChild( parent, func, &data, sizeof( T ) );
    }

    template<typename T>
    zpJob* CreateChild( zpJob* parent, zpJobFunction func, const T& data, zpJob* dependency )
    {
        return CreateChild( parent, func, &data, sizeof( T ), dependency );
    }

    void Run( zpJob* job );
    void Wait( const zpJob* job );
};

//
//
//

class zpCountSplitter
{
public:
    explicit zpCountSplitter( zp_size_t count )
        : m_count( count )
    {
    }

    template<typename T>
    ZP_INLINE zp_bool split( zp_size_t count ) const
    {
        return count > m_count;
    }

private:
    zp_size_t m_count;
};

template<zp_size_t Size>
class zpFixedCountSplitter
{
public:
    template<typename T>
    ZP_INLINE zp_bool split( zp_size_t count ) const
    {
        return count > Size;
    }
};

class zpDataSizeSplitter
{
public:
    explicit zpDataSizeSplitter( zp_size_t size )
        : m_size( size )
    {
    }

    template<typename T>
    ZP_INLINE zp_bool split( zp_size_t count ) const
    {
        return ( count * sizeof( T ) ) > m_size;
    }

private:
    zp_size_t m_size;
};

template<zp_size_t Size>
class zpFixedDataSizeSplitter
{
public:
    template<typename T>
    ZP_INLINE zp_bool split( zp_size_t count ) const
    {
        return ( count * sizeof( T ) ) > Size;
    }
};

//
//
//

template<typename T, typename S>
class zpParallel
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef S splitter_value;
    typedef S& splitter_reference;
    typedef const S& splitter_const_reference;

    typedef void( *ForeachFunction )( pointer data, zp_size_t index );
    typedef void( *ForeachBatchFunction )( pointer data, zp_size_t index, zp_size_t count );

public:
    static zpJob* Foreach( pointer data, zp_size_t count, ForeachFunction func );
    static zpJob* Foreach( pointer data, zp_size_t count, ForeachFunction func, splitter_const_reference splitter );

    static zpJob* ForeachBatch( pointer data, zp_size_t count, zp_size_t batch, ForeachBatchFunction func );
    static zpJob* ForeachBatch( pointer data, zp_size_t count, zp_size_t batch, ForeachBatchFunction func, splitter_const_reference splitter );

private:
    static void ForeachInternalFunction( zpJob* job, const void* data );
    static void ForeachBatchInternalFunction( zpJob* job, const void* data );

private:
    struct ForeachData
    {
        pointer data;
        zp_size_t index;
        zp_size_t count;
        ForeachFunction func;
        splitter_value splitter;
    };

    struct ForeachBatchData
    {
        pointer data;
        zp_size_t index;
        zp_size_t count;
        zp_size_t batch;
        ForeachBatchFunction func;
        splitter_value splitter;
    };
};

template<typename T, typename S>
zpJob* zpParallel<T, S>::Foreach( pointer data, zp_size_t count, ForeachFunction func )
{
    const ForeachData jobData = { data, 0, count, func, splitter_value() };
    return zpJobSystem::Create( ForeachInternalFunction, jobData );
}

template<typename T, typename S>
zpJob* zpParallel<T, S>::Foreach( pointer data, zp_size_t count, ForeachFunction func, splitter_const_reference splitter )
{
    const ForeachData jobData = { data, 0, count, func, splitter };
    return zpJobSystem::Create( ForeachInternalFunction, jobData );
}

template<typename T, typename S>
zpJob* zpParallel<T, S>::ForeachBatch( pointer data, zp_size_t count, zp_size_t batch, ForeachBatchFunction func )
{
    const ForeachBatchData jobData = { data, 0, count, batch, func, splitter_value() };
    return zpJobSystem::Create( ForeachBatchInternalFunction, jobData );
}

template<typename T, typename S>
zpJob* zpParallel<T, S>::ForeachBatch( pointer data, zp_size_t count, zp_size_t batch, ForeachBatchFunction func, splitter_const_reference splitter )
{
    const ForeachBatchData jobData = { data, 0, count, batch, func, splitter };
    return zpJobSystem::Create( ForeachBatchInternalFunction, jobData );
}

template<typename T, typename S>
void zpParallel<T, S>::ForeachInternalFunction( zpJob* job, const void* data )
{
    const ForeachData* jobData = static_cast<const ForeachData*>( data );
    splitter_const_reference splitter = jobData->splitter;

    if( splitter.split<T>( jobData->count ) )
    {
        const zp_size_t leftCount = jobData->count / 2;
        const ForeachData leftJobData = { jobData->data, jobData->index, leftCount, jobData->func, splitter };
        zpJob* leftJob = zpJobSystem::CreateChild( job, ForeachInternalFunction, leftJobData );
        zpJobSystem::Run( leftJob );

        const zp_size_t rightCount = jobData->count - leftCount;
        const ForeachData rightJobData = { jobData->data, jobData->index + leftCount, rightCount, jobData->func, splitter };
        zpJob* rightJob = zpJobSystem::CreateChild( job, ForeachInternalFunction, rightJobData );
        zpJobSystem::Run( rightJob );
    }
    else
    {
        ForeachFunction func = jobData->func;
        const zp_size_t imax = jobData->index + jobData->count;
        for( zp_size_t i = jobData->index; i < imax; ++i )
        {
            func( jobData->data, i );
        }
    }
}

template<typename T, typename S>
void zpParallel<T, S>::ForeachBatchInternalFunction( zpJob* job, const void* data )
{
    const ForeachBatchData* jobData = static_cast<const ForeachBatchData*>( data );
    splitter_const_reference splitter = jobData->splitter;

    if( splitter.split<T>( jobData->count ) )
    {
        const zp_size_t leftCount = jobData->count / 2;
        const ForeachBatchData leftJobData = { jobData->data, jobData->index, leftCount, jobData->batch, jobData->func, splitter };
        zpJob* leftJob = zpJobSystem::CreateChild( job, ForeachBatchInternalFunction, leftJobData );
        zpJobSystem::Run( leftJob );

        const zp_size_t rightCount = jobData->count - leftCount;
        const ForeachBatchData rightJobData = { jobData->data, jobData->index + leftCount, rightCount, jobData->batch, jobData->func, splitter };
        zpJob* rightJob = zpJobSystem::CreateChild( job, ForeachBatchInternalFunction, rightJobData );
        zpJobSystem::Run( rightJob );
    }
    else
    {
        ForeachBatchFunction func = jobData->func;
        const zp_size_t imax = jobData->index + jobData->count;
        for( zp_size_t i = jobData->index; i < imax; )
        {
            const zp_size_t count = i + jobData->batch > imax ? imax - i : jobData->batch;
            func( jobData->data, i, count );
            i += count;
        }
    }
}

#endif // !ZP_JOB_H
