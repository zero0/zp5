#pragma once
#ifndef ZP_HANDLE_H
#define ZP_HANDLE_H

enum zpHandleIdType : zp_hash64
{
    ZP_HANDLE_ID_EMPTY = 0ULL,
    ZP_HANDLE_ID_INVALID = static_cast<zp_hash64>( -1 )
};

//
//
//

template< typename Component >
ZP_SEALED_STRUCT( zpHandleInstance )
{
    typedef Component value_type;

    value_type component;
    zp_size_t refCount;
};

//
//
//

template< typename Component >
ZP_SEALED_CLASS( zpHandle )
{
public:
    typedef zpHandle< Component > handle;

    typedef handle& handle_reference;
    typedef const handle& handle_const_reference;
    typedef handle&& handle_move;

    typedef Component* component_pointer;
    typedef const Component* component_const_pointer;

    typedef zpHandleInstance< Component >* instance_pointer;

    zpHandle()
        : m_instanceId( ZP_HANDLE_ID_EMPTY )
        , m_instance( ZP_NULL )
    {
    }

    zpHandle( instance_pointer instance )
        : m_instanceId( instance ? instance->component.getInstanceId() : ZP_HANDLE_ID_EMPTY )
        , m_instance( instance )
    {
        addRef();
    }

    zpHandle( handle_const_reference other )
        : m_instanceId( other.m_instanceId )
        , m_instance( other.m_instance )
    {
        addRef();
    }

    zpHandle( handle_move other )
        : m_instanceId( other.m_instanceId )
        , m_instance( other.m_instance )
    {
        addRef();
        other.releaseRef();
    }

    ~zpHandle()
    {
        releaseRef();
    }

    handle_reference operator=( handle_const_reference other )
    {
        releaseRef();

        m_instanceId = other.m_instanceId;
        m_instance = other.m_instance;

        addRef();

        return *this;
    }

    handle_reference operator=( handle_move other )
    {
        releaseRef();

        m_instanceId = other.m_instanceId;
        m_instance = other.m_instance;

        addRef();

        other.releaseRef();

        return *this;
    }

    ZP_FORCE_INLINE zp_bool operator==( handle_const_reference other ) const
    {
        return m_instanceId == other.m_instanceId && m_instance == other.m_instance;
    }

    ZP_FORCE_INLINE zp_bool operator!=( handle_const_reference other ) const
    {
        return m_instanceId != other.m_instanceId && m_instance != other.m_instance;
    }

    ZP_FORCE_INLINE component_const_pointer operator->() const
    {
        return get();
    }
    ZP_FORCE_INLINE component_pointer operator->()
    {
        return get();
    }

    ZP_FORCE_INLINE component_const_pointer get() const
    {
        return isValid() ? &m_instance->component : ZP_NULL;
    }

    ZP_FORCE_INLINE component_pointer get()
    {
        return isValid() ? &m_instance->component : ZP_NULL;
    }

    ZP_FORCE_INLINE zp_bool isValid() const
    {
        return m_instance && m_instance->component.getInstanceId() == m_instanceId;
    }

    ZP_FORCE_INLINE operator zp_bool() const
    {
        return isValid();
    }

    ZP_FORCE_INLINE void set( instance_pointer instance, zp_hash64 instanceId )
    {
        releaseRef();

        m_instanceId = instanceId;
        m_instance = instance;

        addRef();
    }

    void release()
    {
        releaseRef();
    }

private:
    ZP_FORCE_INLINE void addRef()
    {
        if( isValid() )
        {
            ++m_instance->refCount;
        }
    }

    ZP_FORCE_INLINE void releaseRef()
    {
        if( isValid() )
        {
            ZP_ASSERT( m_instance->refCount, "" );
            --m_instance->refCount;

            m_instanceId = ZP_HANDLE_ID_EMPTY;
            m_instance = ZP_NULL;
        }
    }

    zp_hash64 m_instanceId;
    instance_pointer m_instance;
};

//
//
//

template< typename Component, typename Allocator = zpDefaultGlobalAllocator >
class zpHandleManager
{
public:
    typedef zpHandle< Component > handle_value;
    typedef zpHandleInstance< Component > instance_value;

    typedef handle_value& handle_reference;
    typedef const handle_value& handle_const_reference;
    typedef handle_value&& handle_move;

    typedef Component* component_pointer;
    typedef const Component* component_const_pointer;

    typedef instance_value* instance_pointer;
    typedef const instance_value* instance_const_pointer;

    typedef Allocator allocator_value;

    zpHandleManager()
        : m_instances()
        , m_newInstanceId( 0 )
        , m_allocator( allocator_value() )
    {

    }
    ~zpHandleManager()
    {
        garbageCollect();
    }

    void garbageCollect()
    {
        for( zp_size_t i = 0, imax = m_instances.size(); i < imax; ++i )
        {
            zpVector< instance_pointer >::iterator t = m_instances.begin() + i;
            instance_pointer inst = *t;
            if( inst->refCount == 0 )
            {
                inst->~instance_value();

                m_instances.eraseAtSwapBack( i );

                --i;
                --imax;

                g_globalAllocator->free( inst );
            }
        }
    }

    void createComponent( handle_reference handle )
    {
        void* ptr = m_allocator.allocate( sizeof( instance_value ) );
        ZP_ASSERT( ptr, "" );

        zp_hash64 instanceId = ++m_newInstanceId;

        instance_pointer instance = static_cast<instance_pointer>( ptr );

        new ( &instance->component ) Component();

        instance->component.setInstanceId( instanceId );
        instance->refCount = 0;

        handle.set( instance, instanceId );

        m_instances.pushBack( instance );
    }

    template< typename Func >
    zp_bool find( Func predecate, handle_reference handle ) const
    {
        zp_bool found = false;

        zpVector< instance_pointer >::const_iterator b = m_instances.begin();
        zpVector< instance_pointer >::const_iterator e = m_instances.end();

        for( ; b != e; ++b )
        {
            if( predecate( *b ) )
            {
                handle.set( *b, ( *b )->component.getInstnaceId() );
                found = true;
                break;
            }
        }

        return found;
    }

    zp_bool findByInstanceId( zp_hash64 instanceId, handle_reference handle ) const
    {
        zp_bool found = find( [ &instanceId ]( instance_const_pointer instance ) -> zp_bool
        {
            return instance->component.getInstanceId() == instanceId;
        }, handle );
        return found;
    }

protected:
    zpVector< instance_pointer > m_instances;

private:
    zp_hash64 m_newInstanceId;
    allocator_value m_allocator;
};

#endif
