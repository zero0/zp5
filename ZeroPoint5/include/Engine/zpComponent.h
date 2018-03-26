#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

enum zpComponentFlags : zp_ulong
{
    ZP_COMPONENT_FLAG_CREATED = 0,
    ZP_COMPONENT_FLAG_STARTED,
    ZP_COMPONENT_FLAG_ENABLE,
    ZP_COMPONENT_FLAG_DIRTY,
    ZP_COMPONENT_FLAG_SHOULD_DESTROY,

    zpComponentFlags_Count,
    ZP_COMPONENT_FLAG_USER_START = zpComponentFlags_Count,
};

template< typename Manager >
ZP_ABSTRACT_CLASS zpComponent
{
    ZP_NON_COPYABLE( zpComponent );
public:

    zpComponent()
        : m_instanceId( 0 )
        , m_flags( 0 )
    {
    }

    virtual ~zpComponent()
    {
    }

    zp_hash64 getInstanceId() const
    {
        return m_instanceId;
    }

    void update( zp_float dt, zp_float rt )
    {
        if( !zp_flag_is_set( m_flags, ZP_COMPONENT_FLAG_STARTED ) )
        {
            onStart();

            zp_flag_mark( m_flags, ZP_COMPONENT_FLAG_STARTED );
        }

        onUpdate( dt, rt );
    }

    void lateUpdate( zp_float dt, zp_float rt )
    {
        onLateUpdate( dt, rt );
    }

    void fixedUpdate( zp_float ft, zp_float rt )
    {
        onFixedUpdate( ft, rt );
    }

    zp_bool isEnabled() const
    {
        return zp_flag_is_set( m_flags, ZP_COMPONENT_FLAG_ENABLE );
    }

    void setEnabled( zp_bool enabled )
    {
        const zp_bool wasEnabled = zp_flag_is_set( m_flags, ZP_COMPONENT_FLAG_ENABLE );
        zp_flag_set( m_flags, ZP_COMPONENT_FLAG_ENABLE, enabled );
        const zp_bool nowEnabled = zp_flag_is_set( m_flags, ZP_COMPONENT_FLAG_ENABLE );

        if( !wasEnabled && nowEnabled )
        {
            onEnabled();
        }
        else if( wasEnabled && !nowEnabled )
        {
            onDisabled();
        }
    }

    void create()
    {
        onCreated();

        zp_flag_mark( m_flags, ZP_COMPONENT_FLAG_CREATED );
    }

    void destroy()
    {
        zp_flag_clear( m_flags, ZP_COMPONENT_FLAG_ENABLE );

        onDisabled();

        onDestroyed();

        zp_flag_mark( m_flags, ZP_COMPONENT_FLAG_SHOULD_DESTROY );
    }

    void setParentObject( const zpObjectHandle& parentObject )
    {
        m_parentObject = parentObject;
    }

    const zpObjectHandle& getParentObject() const
    {
        return m_parentObject;
    }

    zpObjectHandle getParentObject()
    {
        return m_parentObject;
    }

    void setInstanceId( zp_hash64 instanceId )
    {
        m_instanceId = instanceId;
    }

    ZP_INLINE zp_bool isFlagSet( zp_ulong index ) const
    {
        return zp_flag_is_set( m_flags, index );
    }

protected:
    virtual void onStart() {}

    virtual void onEnabled() {}
    virtual void onDisabled() {}

    virtual void onCreated() {}
    virtual void onDestroyed() {}

    virtual void onUpdate( zp_float dt, zp_float rt ) {}
    virtual void onLateUpdate( zp_float dt, zp_float rt ) {}
    virtual void onFixedUpdate( zp_float ft, zp_float rt ) {}

    ZP_INLINE void markFlag( zp_ulong index )
    {
        m_flags = zp_flag_mark( m_flags, index );
    }

    ZP_INLINE void clearFlag( zp_ulong index )
    {
        m_flags = zp_flag_clear( m_flags, index );
    }

    ZP_INLINE void toggleFlag( zp_ulong index )
    {
        m_flags = zp_flag_toggle( m_flags, index );
    }

    ZP_INLINE void setFlag( zp_ulong index, zp_bool set )
    {
        m_flags = zp_flag_set( index, set );
    }

private:

    zpObjectHandle m_parentObject;

    zp_hash64 m_instanceId;
    zp_flag64 m_flags;

    friend Manager;
};

template< typename Component >
ZP_SEALED_STRUCT( zpComponentInstance )
{
    typedef Component value_type;

    value_type component;
    zp_size_t refCount;
};

template< typename Component, typename Instance, typename Manager >
ZP_SEALED_CLASS( zpComponentHandle )
{
public:
    typedef zpComponentHandle< Component, Instance, Manager > handle;

    typedef zpComponentHandle< Component, Instance, Manager >& handle_reference;
    typedef const handle& handle_const_reference;
    typedef handle&& handle_move;

    typedef Component* component_pointer;
    typedef const Component* component_const_pointer;

    typedef Instance* instance_pointer;

    zpComponentHandle()
        : m_instanceId( ZP_HANDLE_ID_EMPTY )
        , m_instance( ZP_NULL )
    {}
    
    zpComponentHandle( handle_const_reference other )
        : m_instanceId( other.m_instanceId )
        , m_instance( other.m_instance )
    {
        addRef();
    }

    zpComponentHandle( handle_move other )
        : m_instanceId( other.m_instanceId )
        , m_instance( other.m_instance )
    {
        addRef();
        other.releaseRef();
    }

    ~zpComponentHandle()
    {
        release();
    }

    handle_reference operator=( handle_const_reference other )
    {
        set( other.m_instanceId, other.m_instance );
        
        return *this;
    }

    handle_reference operator=( handle_move other )
    {
        set( other.m_instanceId, other.m_instance );

        return *this;
    }

    ZP_FORCE_INLINE component_const_pointer operator->() const { return get(); }
    ZP_FORCE_INLINE component_pointer operator->() { return get(); }

    component_const_pointer get() const
    {
        return isValid() ? &m_instance->component : ZP_NULL;
    }

    component_pointer get()
    {
        return isValid() ? &m_instance->component : ZP_NULL;
    }

    zp_bool isValid() const
    {
        return m_instance && m_instance->component.getInstanceId() == m_instanceId;
    }

    void release()
    {
        releaseRef();
    }

    void set( zp_hash64 instanceId, instance_pointer instance )
    {
        releaseRef();

        m_instanceId = instanceId;
        m_instance = instance;

        addRef();
    }

private:
    void addRef()
    {
        if( isValid() )
        {
            ++m_instance->refCount;
        }
    }

    void releaseRef()
    {
        if( isValid() )
        {
            ZP_ASSERT( m_instance->refCount, "" );
            --m_instance->refCount;
        }
    }

    void invalidate()
    {
        m_instanceId = ZP_HANDLE_ID_INVALID;
    }

    zp_hash64 m_instanceId;
    instance_pointer m_instance;

    friend Manager;
};

template< typename Component, typename Instance, typename Manager, typename Handle >
ZP_ABSTRACT_CLASS zpComponentManager
{
    ZP_NON_COPYABLE( zpComponentManager );
public:
    typedef Handle& handle_reference;
    typedef const Handle& handle_const_reference;

    typedef Instance* instance_pointer;
    typedef const Instance* instance_const_pointer;
    typedef Instance** instance_iterator;
    typedef Instance* const* instance_const_iterator;

    zpComponentManager()
        : m_newComponentInstanceId( 0 )
    {
    }

    virtual ~zpComponentManager()
    {
    }

    void update( zp_float dt, zp_float rt )
    {
        ZP_PROFILER_BLOCK();

        onPreUpdate( dt, rt );

        zpVector< instance_pointer >::iterator b = m_activeComponents.begin();
        zpVector< instance_pointer >::iterator e = m_activeComponents.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::value_type p = *b;
            if( !p->component.isFlagSet( ZP_COMPONENT_FLAG_CREATED ) )
            {
                p->component.create();
            }

            if( p->component.isEnabled() )
            {
                p->component.update( dt, rt );
            }
        }

        onPostUpdate( dt, rt );
    }

    void fixedUpdate( zp_float ft, zp_float rt )
    {
        ZP_PROFILER_BLOCK();

        onPreFixedUpdate( ft, rt );

        zpVector< instance_pointer >::iterator b = m_activeComponents.begin();
        zpVector< instance_pointer >::iterator e = m_activeComponents.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::value_type p = *b;
            if( p->component.isEnabled() )
            {
                p->component.fixedUpdate( ft, rt );
            }
        }

        onPostFixedUpdate( ft, rt );
    }

    void lateUpdate( zp_float dt, zp_float rt )
    {
        ZP_PROFILER_BLOCK();

        onPreLateUpdate( dt, rt );

        zpVector< instance_pointer >::iterator b = m_activeComponents.begin();
        zpVector< instance_pointer >::iterator e = m_activeComponents.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::value_type p = *b;
            if( p->component.isEnabled() )
            {
                p->component.lateUpdate( dt, rt );
            }
        }

        onPostLateUpdate( dt, rt );
    }

    void garbageCollect()
    {
        ZP_PROFILER_BLOCK();
        
        for( zp_size_t i = 0, imax = m_activeComponents.size(); i < imax; ++i )
        {
            zpVector< instance_pointer >::iterator t = m_activeComponents.begin() + i;
            if( (*t)->refCount == 0 )
            {
                m_activeComponents.erase( i );

                --i;
                --imax;

                g_globalAllocator.free( *t );
            }
        }
    }

    zp_bool findComponentByInstanceId( zp_hash64 instanceId, handle_reference handle ) const
    {
        ZP_PROFILER_BLOCK();
        
        zp_bool found = false;

        zpVector< instance_pointer >::const_iterator b = m_activeComponents.begin();
        zpVector< instance_pointer >::const_iterator e = m_activeComponents.end();

        for( ; b != e; ++b )
        {
            zpVector< instance_pointer >::pointer t = *b;
            zp_hash64 id = t->component.getInstanceId();
            if( id == instanceId )
            {
                handle.set( id, t );
                found = true;
                break;
            }
        }

        return found;
    }

protected:
    void createComponent( handle_reference handle )
    {
        zp_hash64 instanceId = ++m_newComponentInstanceId;

        instance_pointer instance = static_cast<instance_pointer>( g_globalAllocator.allocate( sizeof( Instance ) ) );

        new ( &instance->component ) Component();

        instance->component.setInstanceId( instanceId );
        instance->refCount = 0;

        handle.set( instanceId, instance );

        m_activeComponents.pushBack( instance );
    }

    virtual void onPreUpdate( zp_float dt, zp_float rt ) {}
    virtual void onPostUpdate( zp_float dt, zp_float rt ) {}

    virtual void onPreLateUpdate( zp_float dt, zp_float rt ) {}
    virtual void onPostLateUpdate( zp_float dt, zp_float rt ) {}

    virtual void onPreFixedUpdate( zp_float ft, zp_float rt ) {}
    virtual void onPostFixedUpdate( zp_float ft, zp_float rt ) {}

private:
    zpVector< instance_pointer > m_activeComponents;
    zp_size_t m_newComponentInstanceId;
};

#endif // !ZP_COMPONENT_H
