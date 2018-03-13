/*
//
//
//
template< typename Manager >
zpComponent< Manager >::zpComponent()
    : m_instanceId( 0 )
    , m_flags( 0 )
{}

template< typename Manager >
zpComponent< Manager >::~zpComponent()
{}

template< typename Manager >
zp_hash64 zpComponent< Manager >::getInstanceId() const
{
    return m_instanceId;
}

template< typename Manager >
void zpComponent< Manager >::update( zp_float dt, zp_float rt )
{
    onUpdate( dt, rt );
}

template< typename Manager >
void zpComponent< Manager >::lateUpdate( zp_float dt, zp_float rt )
{
    onLateUpdate( dt, rt );
}

template< typename Manager >
void zpComponent< Manager >::fixedUpdate( zp_float ft, zp_float rt )
{
    onFixedUpdate( ft, rt );
}

template< typename Manager >
zp_bool zpComponent< Manager >::isEnabled() const
{
    return ( m_flags & ( 1 << 0ull ) ) != 0;
}

template< typename Manager >
void zpComponent< Manager >::setEnabled( zp_bool enabled )
{
    const zp_bool wasEnabled = isEnabled();
    m_flags ^= ( ( enabled ? -1 : 0 ) ^ m_flags ) & ( 1 << 0ull );
    const zp_bool nowEnabled = isEnabled();

    if( !wasEnabled && nowEnabled )
    {
        onEnabled();
    }
    else if( wasEnabled && !nowEnabled )
    {
        onDisabled();
    }
}

template< typename Manager >
void zpComponent< Manager >::create()
{
    onCreated();
}

template< typename Manager >
void zpComponent< Manager >::destroy()
{
    onDestroyed();
}

template< typename Manager >
const zpObjectHandle& zpComponent< Manager >::getParentObject() const
{
    return m_parentObject;
}

template< typename Manager >
zpObjectHandle zpComponent< Manager >::getParentObject()
{
    return m_parentObject;
}

template< typename Manager >
void zpComponent< Manager >::setInstanceId( zp_hash64 instanceId )
{
    m_instanceId = instanceId;
}

//
//
//

template< typename Component, typename Instance, typename Manager >
zpComponentHandle< Component, Instance, Manager >::zpComponentHandle()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
    , m_instance( ZP_NULL )
{}

template< typename Component, typename Instance, typename Manager >
zpComponentHandle< Component, Instance, Manager >::zpComponentHandle( handle_const_reference other )
    : m_instanceId( other.m_instanceId )
    , m_instance( other.m_instance )
{
    addRef();
}

template< typename Component, typename Instance, typename Manager >
zpComponentHandle< Component, Instance, Manager >::zpComponentHandle( handle_move other )
    : m_instanceId( other.m_instanceId )
    , m_instance( other.m_instance )
{
    addRef();
    other.releaseRef();
}

template< typename Component, typename Instance, typename Manager >
zpComponentHandle< Component, Instance, Manager >::~zpComponentHandle()
{
    release();
}

template< typename Component, typename Instance, typename Manager >
typename zpComponentHandle< Component, Instance, Manager >::handle_reference zpComponentHandle< Component, Instance, Manager >::operator=( handle_const_reference other )
{
    set( other.m_instanceId, other.m_instance );

    return *this;
}

template< typename Component, typename Instance, typename Manager >
typename zpComponentHandle< Component, Instance, Manager >::handle_reference zpComponentHandle< Component, Instance, Manager >::operator=( handle_move other )
{
    set( other.m_instanceId, other.m_instance );

    other.releaseRef();

    return *this;
}

template< typename Component, typename Instance, typename Manager >
typename zpComponentHandle< Component, Instance, Manager >::component_const_pointer zpComponentHandle< Component, Instance, Manager >::get() const
{
    return isValid() ? &m_instance->component : ZP_NULL;
}

template< typename Component, typename Instance, typename Manager >
typename zpComponentHandle< Component, Instance, Manager >::component_pointer zpComponentHandle< Component, Instance, Manager >::get()
{
    return isValid() ? &m_instance->component : ZP_NULL;
}

template< typename Component, typename Instance, typename Manager >
zp_bool zpComponentHandle< Component, Instance, Manager >::isValid() const
{
    return m_instance && m_instance->component.getInstanceId() == m_instanceId;
}

template< typename Component, typename Instance, typename Manager >
void zpComponentHandle< Component, Instance, Manager >::release()
{
    releaseRef();
}

template< typename Component, typename Instance, typename Manager >
void zpComponentHandle< Component, Instance, Manager >::addRef()
{
    if( isValid() )
    {
        ++m_instance->refCount;
    }
}

template< typename Component, typename Instance, typename Manager >
void zpComponentHandle< Component, Instance, Manager >::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_instance->refCount != 0, "" );
        --m_instance->refCount;
    }
}

template< typename Component, typename Instance, typename Manager >
void zpComponentHandle< Component, Instance, Manager >::set( zp_hash64 instanceId, instance_pointer instance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_instance = instance;

    addRef();
}

template< typename Component, typename Instance, typename Manager >
void zpComponentHandle< Component, Instance, Manager >::invalidate()
{
    m_instanceId = ZP_HANDLE_ID_INVALID;
}

//
//
//

template< typename Component, typename Instance, typename Manager, typename Handle >
zpComponentManager< Component, Instance, Manager, Handle >::zpComponentManager()
    : m_newComponentInstanceId( 0 )
{}

template< typename Component, typename Instance, typename Manager, typename Handle >
zpComponentManager< Component, Instance, Manager, Handle >::~zpComponentManager()
{}

template< typename Component, typename Instance, typename Manager, typename Handle >
void zpComponentManager< Component, Instance, Manager, Handle >::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    instance_iterator b = m_activeComponents.begin();
    instance_iterator e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        instance_pointer p = *b;
        if( p->component.isEnabled() )
        {
            p->component.update( dt, rt );
        }
    }
}

template< typename Component, typename Instance, typename Manager, typename Handle >
void zpComponentManager< Component, Instance, Manager, Handle >::fixedUpdate( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    instance_iterator b = m_activeComponents.begin();
    instance_iterator e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        instance_pointer p = *b;
        if( p->component.isEnabled() )
        {
            p->component.fixedUpdate( dt, rt );
        }
    }
}

template< typename Component, typename Instance, typename Manager, typename Handle >
void zpComponentManager< Component, Instance, Manager, Handle >::lateUpdate( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    instance_iterator b = m_activeComponents.begin();
    instance_iterator e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        instance_pointer p = *b;
        if( p->component.isEnabled() )
        {
            p->component.lateUpdate( dt, rt );
        }
    }
}

template< typename Component, typename Instance, typename Manager, typename Handle >
void zpComponentManager< Component, Instance, Manager, Handle >::garbageCollect()
{}

template< typename Component, typename Instance, typename Manager, typename Handle >
zp_bool zpComponentManager< Component, Instance, Manager, Handle >::findComponentByInstanceId( zp_hash64 instanceId, handle_reference handle ) const
{
    zp_bool found = false;

    instance_const_iterator b = m_activeComponents.begin();
    instance_const_iterator e = m_activeComponents.end();
    
    for( ; b != e; ++b )
    {
        instance_pointer t = *b;
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

template< typename Component, typename Instance, typename Manager, typename Handle >
void zpComponentManager< Component, Instance, Manager, Handle >::createComponent( handle_reference handle )
{
    ZP_PROFILER_BLOCK();
    
    zp_hash64 instanceId = ++m_newComponentInstanceId;

    instance_pointer instance = static_cast<instance_pointer>( g_globalAllocator.allocate( sizeof( Instance ) ) );
    
    new ( &instance->component ) Component();

    instance->component.setInstanceId( instanceId );
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_activeComponents.pushBack( instance );
}
*/