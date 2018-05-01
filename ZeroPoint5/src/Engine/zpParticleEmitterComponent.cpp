#include "Engine/zpEngine.h"
#include <new>

enum : zp_size_t
{
    ZP_MESH_RENDERER_FLAG_ENABLED,
    ZP_MESH_RENDERER_FLAG_DIRTY,
};

zpParticleEmitterComponent::zpParticleEmitterComponent()
    : m_instanceID( 0 )
    , m_flags( ZP_MESH_RENDERER_FLAG_DIRTY )
    , m_parentObject()
{
}

zpParticleEmitterComponent::~zpParticleEmitterComponent()
{
}

void zpParticleEmitterComponent::setParentObject( const zpObjectHandle& parent )
{
    m_parentObject = parent;

    m_flags |= ZP_MESH_RENDERER_FLAG_DIRTY;
}

const zpObjectHandle& zpParticleEmitterComponent::getParentObject() const
{
    return m_parentObject;
}

void zpParticleEmitterComponent::update( zp_float dt, zp_float rt )
{
    if( m_flags & ( 1 << ZP_MESH_RENDERER_FLAG_DIRTY ) )
    {
        m_flags &= ~( 1 << ZP_MESH_RENDERER_FLAG_DIRTY );
    }
}

zp_bool zpParticleEmitterComponent::isEnabled() const
{
    return ( m_flags & ( 1 << ZP_MESH_RENDERER_FLAG_ENABLED ) ) == ( 1 << ZP_MESH_RENDERER_FLAG_ENABLED );
}

void zpParticleEmitterComponent::setEnabled( zp_bool enabled )
{
    m_flags ^= ( ( enabled ? -1 : 0 ) ^ m_flags ) & ( 1 << ZP_MESH_RENDERER_FLAG_ENABLED );
}

zp_hash64 zpParticleEmitterComponent::getInstanceId() const
{
    return m_instanceID;
}

void zpParticleEmitterComponent::setInstanceId( zp_hash64 instanceId )
{
    m_instanceID = instanceId;
}

//
//
//

zpParticleEmitterComponentHandle::zpParticleEmitterComponentHandle()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
    , m_particleEmitterInstance( ZP_NULL )
{

}

zpParticleEmitterComponentHandle::zpParticleEmitterComponentHandle( const zpParticleEmitterComponentHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_particleEmitterInstance( other.m_particleEmitterInstance )
{
    addRef();
}

zpParticleEmitterComponentHandle::zpParticleEmitterComponentHandle( zpParticleEmitterComponentHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_particleEmitterInstance( other.m_particleEmitterInstance )
{
    addRef();
    other.release();
}

zpParticleEmitterComponentHandle::~zpParticleEmitterComponentHandle()
{
    release();
}

zpParticleEmitterComponentHandle& zpParticleEmitterComponentHandle::operator=( const zpParticleEmitterComponentHandle& other )
{
    set( other.m_instanceId, other.m_particleEmitterInstance );

    return *this;
}

zpParticleEmitterComponentHandle& zpParticleEmitterComponentHandle::operator=( zpParticleEmitterComponentHandle&& other )
{
    set( other.m_instanceId, other.m_particleEmitterInstance );

    other.release();

    return *this;
}

const zpParticleEmitterComponent* zpParticleEmitterComponentHandle::get() const
{
    return isValid() ? &m_particleEmitterInstance->particleEmitter : ZP_NULL;
}

zpParticleEmitterComponent* zpParticleEmitterComponentHandle::get()
{
    return isValid() ? &m_particleEmitterInstance->particleEmitter : ZP_NULL;
}

zp_bool zpParticleEmitterComponentHandle::isValid() const
{
    return m_particleEmitterInstance != ZP_NULL && m_particleEmitterInstance->particleEmitter.getInstanceId() == m_instanceId;
}

void zpParticleEmitterComponentHandle::release()
{
    releaseRef();
}

void zpParticleEmitterComponentHandle::addRef()
{
    if( isValid() )
    {
        ++m_particleEmitterInstance->refCount;
    }
}

void zpParticleEmitterComponentHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_particleEmitterInstance->refCount != 0, "" );
        --m_particleEmitterInstance->refCount;
    }
}

void zpParticleEmitterComponentHandle::set( zp_hash64 instanceId, zpParticleEmitterComponentInstance* particleEmitterInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_particleEmitterInstance = particleEmitterInstance;

    addRef();
}

//
//
//

zpParticleEmitterComponentManager::zpParticleEmitterComponentManager()
    : m_activeComponents( 4 )
    , m_newParticleEmitterComponentInstanceId( ZP_HANDLE_ID_EMPTY )
{

}

zpParticleEmitterComponentManager::~zpParticleEmitterComponentManager()
{

}

void zpParticleEmitterComponentManager::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    zpParticleEmitterComponentInstance** b = m_activeComponents.begin();
    zpParticleEmitterComponentInstance** e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        zpParticleEmitterComponentInstance* t = *b;
        t->particleEmitter.update( dt, rt );
    }
}

void zpParticleEmitterComponentManager::createParticleEmitterComponent( zpParticleEmitterComponentHandle& handle )
{
    zp_hash64 instanceId = ++m_newParticleEmitterComponentInstanceId;

    zpParticleEmitterComponentInstance* instance = static_cast<zpParticleEmitterComponentInstance*>( g_globalAllocator->allocate( sizeof( zpParticleEmitterComponentInstance ) ) );

    new ( &instance->particleEmitter ) zpParticleEmitterComponent();

    instance->particleEmitter.setInstanceId( instanceId );
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_activeComponents.pushBack( instance );
}

void zpParticleEmitterComponentManager::createParticleEmitterComponent( zpParticleEmitterComponentHandle& handle, zpParticleEmitterComponentDesc* desc )
{
    createParticleEmitterComponent( handle );

    if( desc )
    {
    }
}

void zpParticleEmitterComponentManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_activeComponents.size(); i != imax; ++i )
    {
        zpParticleEmitterComponentInstance* b = m_activeComponents[ i ];
        if( b->refCount == 0 )
        {
            ( &b->particleEmitter )->~zpParticleEmitterComponent();
            g_globalAllocator->free( b );

            m_activeComponents.erase( i );

            --i;
            --imax;
        }
    }
}

zp_bool zpParticleEmitterComponentManager::findComponentByID( zp_hash64 instanceId, zpParticleEmitterComponentHandle& handle ) const
{
    zp_bool found = false;
    zpParticleEmitterComponentInstance* const* b = m_activeComponents.begin();
    zpParticleEmitterComponentInstance* const* e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        zpParticleEmitterComponentInstance* t = *b;
        const zp_hash64 id = t->particleEmitter.getInstanceId();
        if( id == instanceId )
        {
            handle.set( id, t );
            found = true;
            break;
        }
    }

    return found;
}
