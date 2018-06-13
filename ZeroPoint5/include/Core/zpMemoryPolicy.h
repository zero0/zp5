#pragma once
#ifndef ZP_MEMORY_POLICY_H
#define ZP_MEMORY_POLICY_H

template<zp_size_t StackSize = 32>
class zpStackMemoryPolicy
{
public:
    zpStackMemoryPolicy()
    {
    }
    ~zpStackMemoryPolicy()
    {
    }

    void setup( void* memory, zp_size_t size )
    {
        m_memory = memory;
        m_totalMemory = size;
        m_totalMemoryUsed = 0;
        m_stackCount = 0;
    }

    void teardown()
    {
        m_memory = ZP_NULL;
        m_totalMemory = 0;
        m_totalMemoryUsed = 0;
        m_stackCount = 0;
    }

    void* allocate( zp_size_t size )
    {
        ZP_ASSERT( m_stackCount != StackSize, "" );
        ZP_ASSERT( ( m_totalMemoryUsed + size ) < m_totalMemory, "" );

        void* ptr = ZP_NULL;
        if( m_stackCount != StackSize && ( m_totalMemoryUsed + size ) < m_totalMemory )
        {
            zpStackBlock* block = m_stack + m_stackCount;

            ptr = static_cast<zp_byte*>( m_memory ) + m_totalMemoryUsed;
            block->memory = ptr;
            block->size = size;

            ++m_stackCount;
            m_totalMemoryUsed += size;
        }

        return ptr;
    }

    void free( const void* ptr )
    {
        ZP_ASSERT( m_stackCount > 0, "" );

        zpStackBlock* block = m_stack + ( static_cast<zp_ptrdiff_t>( m_stackCount ) - 1 );
        ZP_ASSERT( block->memory == ptr, "" );

        m_totalMemoryUsed -= block->size;
        --m_stackCount;
    }

    zp_size_t getMemoryUsed() const
    {
        return m_totalMemoryUsed;
    }

private:
    void* m_memory;
    zp_size_t m_stackCount;
    zp_size_t m_totalMemory;
    zp_size_t m_totalMemoryUsed;

    struct zpStackBlock
    {
        void* memory;
        zp_size_t size;
    } m_stack[ StackSize ];
};

//
//
//

class zpTLFSMemoryPolicy
{
public:
    zpTLFSMemoryPolicy();
    ~zpTLFSMemoryPolicy();

    void setup( void* memory, zp_size_t size );
    void teardown();

    void* allocate( zp_size_t size );
    void free( const void* ptr );

    zp_size_t getMemoryUsed() const;

private:
    zp_tlsf_t m_tlsf;

    zp_size_t m_totalMemoryUsed;
};

#endif // !ZP_MEMORY_POLICY_H
