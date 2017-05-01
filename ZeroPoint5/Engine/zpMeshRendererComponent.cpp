#include "zpEngine.h"
#include <new>

enum : zp_size_t
{
    ZP_MESH_RENDERER_FLAG_ENABLED,
    ZP_MESH_RENDERER_FLAG_DIRTY,
};

zpMeshRendererComponent::zpMeshRendererComponent()
    : m_instanceID( 0 )
    , m_flags( ZP_MESH_RENDERER_FLAG_DIRTY )
    , m_parentObject()
{
}

zpMeshRendererComponent::~zpMeshRendererComponent()
{
}

void zpMeshRendererComponent::setParentObject( const zpObjectHandle& parent )
{
    m_parentObject = parent;

    m_flags |= ZP_MESH_RENDERER_FLAG_DIRTY;
}

const zpObjectHandle& zpMeshRendererComponent::getParentObject() const
{
    return m_parentObject;
}

void zpMeshRendererComponent::update( zp_float dt, zp_float rt )
{
    if( m_flags & ( 1 << ZP_MESH_RENDERER_FLAG_DIRTY ) )
    {
        m_flags &= ~( 1 << ZP_MESH_RENDERER_FLAG_DIRTY );
    }
}

void zpMeshRendererComponent::render( zpRenderingContext* ctx )
{
    if( m_mesh.isValid() && m_material.isValid() )
    {
        const zpMatrix4fData& transform = m_parentObject->getAllComponents()->transform->getWorldMatrix();

        ctx->drawMesh( m_renderLayer, transform, m_mesh, m_material );
    }
}


zp_bool zpMeshRendererComponent::isEnabled() const
{
    return ( m_flags & ( 1 << ZP_MESH_RENDERER_FLAG_ENABLED ) ) == ( 1 << ZP_MESH_RENDERER_FLAG_ENABLED );
}

void zpMeshRendererComponent::setEnabled( zp_bool enabled )
{
    m_flags ^= ( ( enabled ? -1 : 0 ) ^ m_flags ) & ( 1 << ZP_MESH_RENDERER_FLAG_ENABLED );
}

zp_hash64 zpMeshRendererComponent::getInstanceId() const
{
    return m_instanceID;
}

void zpMeshRendererComponent::setInstanceId( zp_hash64 instanceId )
{
    m_instanceID = instanceId;
}

void zpMeshRendererComponent::setMesh( const zpMeshHandle& mesh )
{
    m_mesh = mesh;
}
const zpMeshHandle& zpMeshRendererComponent::getMesh() const
{
    return m_mesh;
}

void zpMeshRendererComponent::setMaterial( const zpMaterialHandle& material )
{
    m_material = material;
}
const zpMaterialHandle& zpMeshRendererComponent::getMaterial() const
{
    return m_material;
}

//
//
//

zpMeshRendererComponentHandle::zpMeshRendererComponentHandle()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
    , m_meshRendererInstance( ZP_NULL )
{

}

zpMeshRendererComponentHandle::zpMeshRendererComponentHandle( const zpMeshRendererComponentHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_meshRendererInstance( other.m_meshRendererInstance )
{
    addRef();
}

zpMeshRendererComponentHandle::zpMeshRendererComponentHandle( zpMeshRendererComponentHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_meshRendererInstance( other.m_meshRendererInstance )
{
    addRef();
    other.release();
}

zpMeshRendererComponentHandle::~zpMeshRendererComponentHandle()
{
    release();
}

zpMeshRendererComponentHandle& zpMeshRendererComponentHandle::operator=( const zpMeshRendererComponentHandle& other )
{
    set( other.m_instanceId, other.m_meshRendererInstance );

    return *this;
}

zpMeshRendererComponentHandle& zpMeshRendererComponentHandle::operator=( zpMeshRendererComponentHandle&& other )
{
    set( other.m_instanceId, other.m_meshRendererInstance );

    other.release();

    return *this;
}

const zpMeshRendererComponent* zpMeshRendererComponentHandle::get() const
{
    return isValid() ? &m_meshRendererInstance->meshRenderer : ZP_NULL;
}

zpMeshRendererComponent* zpMeshRendererComponentHandle::get()
{
    return isValid() ? &m_meshRendererInstance->meshRenderer : ZP_NULL;
}

zp_bool zpMeshRendererComponentHandle::isValid() const
{
    return m_meshRendererInstance != ZP_NULL && m_meshRendererInstance->meshRenderer.getInstanceId() == m_instanceId;
}

void zpMeshRendererComponentHandle::release()
{
    releaseRef();
}

void zpMeshRendererComponentHandle::addRef()
{
    if( isValid() )
    {
        ++m_meshRendererInstance->refCount;
    }
}

void zpMeshRendererComponentHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_meshRendererInstance->refCount != 0, "" );
        --m_meshRendererInstance->refCount;
    }
}

void zpMeshRendererComponentHandle::set( zp_hash64 instanceId, zpMeshRendererComponentInstance* meshRendererInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_meshRendererInstance = meshRendererInstance;

    addRef();
}

//
//
//

zpMeshRendererComponentManager::zpMeshRendererComponentManager()
    : m_activeComponents( 4 )
    , m_newMeshRendererComponentInstanceId( ZP_HANDLE_ID_EMPTY )
{

}

zpMeshRendererComponentManager::~zpMeshRendererComponentManager()
{

}

void zpMeshRendererComponentManager::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    zpMeshRendererComponentInstance** b = m_activeComponents.begin();
    zpMeshRendererComponentInstance** e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        zpMeshRendererComponentInstance* t = *b;
        t->meshRenderer.update( dt, rt );
    }
}

void zpMeshRendererComponentManager::render( zpRenderingContext* ctx )
{
    ZP_PROFILER_BLOCK();
    
    zpMeshRendererComponentInstance** b = m_activeComponents.begin();
    zpMeshRendererComponentInstance** e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        zpMeshRendererComponentInstance* t = *b;
        t->meshRenderer.render( ctx );
    }
}

void zpMeshRendererComponentManager::createMeshRendererComponent( zpMeshRendererComponentHandle& handle )
{
    zp_hash64 instanceId = ++m_newMeshRendererComponentInstanceId;

    zpMeshRendererComponentInstance* instance = static_cast<zpMeshRendererComponentInstance*>( g_globalAllocator.allocate( sizeof( zpMeshRendererComponentInstance ) ) );

    new ( &instance->meshRenderer ) zpMeshRendererComponent();

    instance->meshRenderer.setInstanceId( instanceId );
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_activeComponents.pushBack( instance );
}

void zpMeshRendererComponentManager::createMeshRendererComponent( zpMeshRendererComponentHandle& handle, zpMeshRendererComponentDesc* desc )
{
    createMeshRendererComponent( handle );

    if( desc )
    {
    }
}

void zpMeshRendererComponentManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_activeComponents.size(); i != imax; ++i )
    {
        zpMeshRendererComponentInstance* b = m_activeComponents[ i ];
        if( b->refCount == 0 )
        {
            ( &b->meshRenderer )->~zpMeshRendererComponent();
            g_globalAllocator.free( b );

            m_activeComponents.erase( i );

            --i;
            --imax;
        }
    }
}

zp_bool zpMeshRendererComponentManager::findComponentByID( zp_hash64 instanceId, zpMeshRendererComponentHandle& handle ) const
{
    zp_bool found = false;
    zpMeshRendererComponentInstance* const* b = m_activeComponents.begin();
    zpMeshRendererComponentInstance* const* e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        zpMeshRendererComponentInstance* t = *b;
        const zp_hash64 id = t->meshRenderer.getInstanceId();
        if( id == instanceId )
        {
            handle.set( id, t );
            found = true;
            break;
        }
    }

    return found;
}
