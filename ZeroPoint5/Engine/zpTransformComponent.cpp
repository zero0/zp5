#include "zpEngine.h"
#include <new>

enum : zp_size_t
{
    ZP_TRANSFORM_FLAG_ENABLED,
    ZP_TRANSFORM_FLAG_DIRTY,
};

zpTransformComponent::zpTransformComponent()
    : m_localPosition()
    , m_localRotation()
    , m_localScale()
    , m_localMatrix()
    , m_worldMatrix()
    , m_instanceID( 0 )
    , m_flags( 0 )
    , m_parentTransfrom()
    , m_parentObject()
{
    setLocalPositionRotationScale( { 0, 0, 0, 1 }, { 0, 0, 0, 1 }, { 1, 1, 1, 0 } );
}

zpTransformComponent::~zpTransformComponent()
{
}

void zpTransformComponent::setParentObject( const zpObjectHandle& parent )
{
    m_parentObject = parent;

    m_flags |= ( 1 << ZP_TRANSFORM_FLAG_DIRTY );
}

const zpObjectHandle& zpTransformComponent::getParentObject() const
{
    return m_parentObject;
}

void zpTransformComponent::setLocalPosition( const zpVector4fData& position )
{
    m_localPosition = position;

    ZP_ALIGN16 zpVector4fData pos = m_localPosition;
    ZP_ALIGN16 zpQuaternion4fData rot = m_localRotation;
    ZP_ALIGN16 zpVector4fData scl = m_localScale;

    zpVector4f p = zpMath::Vector4Load4( pos.m );
    zpQuaternion4f r = zpMath::QuaternionLoad4( rot.m );
    zpVector4f s = zpMath::Vector4Load4( scl.m );

    ZP_ALIGN16 zpMatrix4fData mat;
    zpMatrix4f m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, mat.m );
    m_localMatrix = mat;

    m_flags |= ( 1 << ZP_TRANSFORM_FLAG_DIRTY );
}

void zpTransformComponent::setLocalPositionRotation( const zpVector4fData& position, const zpQuaternion4fData& rotation )
{
    m_localPosition = position;
    m_localRotation = rotation;

    ZP_ALIGN16 zpVector4fData pos = m_localPosition;
    ZP_ALIGN16 zpQuaternion4fData rot = m_localRotation;
    ZP_ALIGN16 zpVector4fData scl = m_localScale;

    zpVector4f p = zpMath::Vector4Load4( pos.m );
    zpQuaternion4f r = zpMath::QuaternionLoad4( rot.m );
    zpVector4f s = zpMath::Vector4Load4( scl.m );

    ZP_ALIGN16 zpMatrix4fData mat;
    zpMatrix4f m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, mat.m );
    m_localMatrix = mat;

    m_flags |= ( 1 << ZP_TRANSFORM_FLAG_DIRTY );
}

void zpTransformComponent::setLocalPositionRotationScale( const zpVector4fData& position, const zpQuaternion4fData& rotation, const zpVector4fData& scale )
{
    m_localPosition = position;
    m_localRotation = rotation;
    m_localScale = scale;

    ZP_ALIGN16 zpVector4fData pos = m_localPosition;
    ZP_ALIGN16 zpQuaternion4fData rot = m_localRotation;
    ZP_ALIGN16 zpVector4fData scl = m_localScale;

    zpVector4f p = zpMath::Vector4Load4( pos.m );
    zpQuaternion4f r = zpMath::QuaternionLoad4( rot.m );
    zpVector4f s = zpMath::Vector4Load4( scl.m );

    ZP_ALIGN16 zpMatrix4fData mat;
    zpMatrix4f m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, mat.m );
    m_localMatrix = mat;

    m_flags |= ( 1 << ZP_TRANSFORM_FLAG_DIRTY );
}

const zpVector4fData& zpTransformComponent::getLocalPosition() const
{
    return m_localPosition;
}

const zpQuaternion4fData& zpTransformComponent::getLocalRotation() const
{
    return m_localRotation;
}

const zpVector4fData& zpTransformComponent::getLocalScale() const
{
    return m_localScale;
}

const zpMatrix4fData& zpTransformComponent::getLocalMatrix() const
{
    return m_localMatrix;
}

const zpMatrix4fData& zpTransformComponent::getWorldMatrix() const
{
    return m_worldMatrix;
}

const zpTransformComponentHandle& zpTransformComponent::getParentTransform() const
{
    return m_parentTransfrom;
}

zpTransformComponentHandle& zpTransformComponent::getParentTransform()
{
    return m_parentTransfrom;
}

void zpTransformComponent::setParentTransform( const zpTransformComponentHandle& parent )
{
    m_parentTransfrom = parent;

    m_flags |= ( 1 << ZP_TRANSFORM_FLAG_DIRTY );
}

void zpTransformComponent::update( zp_float dt, zp_float rt )
{
    if( m_flags & ( 1 << ZP_TRANSFORM_FLAG_DIRTY ) )
    {
        ZP_ALIGN16 zpMatrix4fData mat = m_localMatrix;

        zpMatrix4f world = zpMath::MatrixLoad4( mat.m );
        zpTransformComponentHandle parent = m_parentTransfrom;

        while( parent.isValid() )
        {
            mat = parent->getLocalMatrix();

            zpMatrix4f local = zpMath::MatrixLoad4( mat.m );
            world = zpMath::MatrixMul( world, local );
            parent = parent->getParentTransform();
        }

        zpMath::MatrixStore4( world, mat.m );

        m_worldMatrix = mat;

        m_flags &= ~( 1 << ZP_TRANSFORM_FLAG_DIRTY );
    }
}

zp_bool zpTransformComponent::isEnabled() const
{
    return ( m_flags & ( 1 << ZP_TRANSFORM_FLAG_ENABLED ) ) == ( 1 << ZP_TRANSFORM_FLAG_ENABLED );
}

void zpTransformComponent::setEnabled( zp_bool enabled )
{
    m_flags ^= ( ( enabled ? -1 : 0 ) ^ m_flags ) & ( 1 << ZP_TRANSFORM_FLAG_ENABLED );
}

zp_hash64 zpTransformComponent::getInstanceId() const
{
    return m_instanceID;
}

void zpTransformComponent::setInstanceId( zp_hash64 instanceId )
{
    m_instanceID = instanceId;
}

//
//
//

zpTransformComponentHandle::zpTransformComponentHandle()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
    , m_transformInstance( ZP_NULL )
{

}

zpTransformComponentHandle::zpTransformComponentHandle( const zpTransformComponentHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_transformInstance( other.m_transformInstance )
{
    addRef();
}

zpTransformComponentHandle::zpTransformComponentHandle( zpTransformComponentHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_transformInstance( other.m_transformInstance )
{
    addRef();
    other.release();
}

zpTransformComponentHandle::~zpTransformComponentHandle()
{
    release();
}

zpTransformComponentHandle& zpTransformComponentHandle::operator=( const zpTransformComponentHandle& other )
{
    set( other.m_instanceId, other.m_transformInstance );

    return *this;
}

zpTransformComponentHandle& zpTransformComponentHandle::operator=( zpTransformComponentHandle&& other )
{
    set( other.m_instanceId, other.m_transformInstance );

    other.release();

    return *this;
}

const zpTransformComponent* zpTransformComponentHandle::get() const
{
    return isValid() ? &m_transformInstance->transform : ZP_NULL;
}

zpTransformComponent* zpTransformComponentHandle::get()
{
    return isValid() ? &m_transformInstance->transform : ZP_NULL;
}

zp_bool zpTransformComponentHandle::isValid() const
{
    return m_transformInstance != ZP_NULL && m_transformInstance->transform.getInstanceId() == m_instanceId;
}

void zpTransformComponentHandle::release()
{
    releaseRef();
}

void zpTransformComponentHandle::addRef()
{
    if( isValid() )
    {
        ++m_transformInstance->refCount;
    }
}

void zpTransformComponentHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_transformInstance->refCount != 0, "" );
        --m_transformInstance->refCount;
    }
}

void zpTransformComponentHandle::set( zp_hash64 instanceId, zpTransformComponentInstance* transformInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_transformInstance = transformInstance;

    addRef();
}

//
//
//

zpTransformComponentManager::zpTransformComponentManager()
    : m_activeComponents( 4 )
    , m_newTransformComponentInstanceId( ZP_HANDLE_ID_EMPTY )
{

}

zpTransformComponentManager::~zpTransformComponentManager()
{

}

void zpTransformComponentManager::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();
   
    zpTransformComponentInstance** b = m_activeComponents.begin();
    zpTransformComponentInstance** e = m_activeComponents.end();

    for( ; b != e; ++b )
    {
        zpTransformComponentInstance* t = *b;
        t->transform.update( dt, rt );
    }
}

void zpTransformComponentManager::createTransformComponent( zpTransformComponentHandle& handle )
{
    zp_hash64 instanceId = ++m_newTransformComponentInstanceId;

    zpTransformComponentInstance* instance = static_cast<zpTransformComponentInstance*>( g_globalAllocator.allocate( sizeof( zpTransformComponentInstance ) ) );
    
    new ( &instance->transform ) zpTransformComponent();

    instance->transform.setInstanceId( instanceId );
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_activeComponents.pushBack( instance );
}

void zpTransformComponentManager::createTransformComponent( zpTransformComponentHandle& handle, zpTransformComponentDesc* desc )
{
    createTransformComponent( handle );

    if( desc )
    {
        handle->setLocalPositionRotationScale( desc->localPosition, desc->localRotation, desc->localScale );
        handle->setParentObject( desc->parentObject );
        handle->setParentTransform( desc->parentObject->getAllComponents()->transform );
    }
}

void zpTransformComponentManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_activeComponents.size(); i != imax; ++i )
    {
        zpTransformComponentInstance* b = m_activeComponents[ i ];
        if( b->refCount == 0 )
        {
            ( &b->transform )->~zpTransformComponent();
            g_globalAllocator.free( b );

            m_activeComponents.erase( i );

            --i;
            --imax;
        }
    }
}

zp_bool zpTransformComponentManager::findComponentByID( zp_hash64 instanceId, zpTransformComponentHandle& handle ) const
{
    zp_bool found = false;
    zpTransformComponentInstance* const* b = m_activeComponents.begin();
    zpTransformComponentInstance* const* e = m_activeComponents.end();
    
    for( ; b != e; ++b )
    {
        zpTransformComponentInstance* t = *b;
        zp_hash64 id = t->transform.getInstanceId();
        if( id == instanceId )
        {
            handle.set( id, t );
            found = true;
            break;
        }
    }

    return found;
}
