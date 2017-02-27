#include "zpEngine.h"
#include <new>

enum : zp_size_t
{
    ZP_TRANSFORM_FLAG_ENABLED = 1 << 0,
    ZP_TRANSFORM_FLAG_DIRTY =   1 << 1,
};

zpTransformComponent::zpTransformComponent()
    : m_localPosition()
    , m_localRotation()
    , m_localScale()
    , m_localMatrix()
    , m_worldMatrix()
    , m_instanceID( 0 )
    , m_flags( ZP_TRANSFORM_FLAG_DIRTY )
    , m_parentTransfrom()
    , m_parentObject()
{
    zpVector4fData ip = { 0, 0, 0, 1 };
    zpQuaternion4fData ir = { 0, 0, 0, 1 };
    zpVector4fData is = { 1, 1, 1, 0 };
    m_localPosition = ip;
    m_localRotation = ir;
    m_localScale = is;
}

zpTransformComponent::~zpTransformComponent()
{
}

void zpTransformComponent::setParentObject( const zpObjectHandle& parent )
{
    m_parentObject = parent;

    m_flags |= ZP_TRANSFORM_FLAG_DIRTY;
}

const zpObjectHandle& zpTransformComponent::getParentObject() const
{
    return m_parentObject;
}

void zpTransformComponent::setLocalPosition( const zpVector4fData& position )
{
    m_localPosition = position;

    zpVector4f p = zpMath::Vector4Load4( m_localPosition.m );
    zpQuaternion4f r = zpMath::QuaternionLoad4( m_localRotation.m );
    zpVector4f s = zpMath::Vector4Load4( m_localScale.m );

    zpMatrix4f m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, m_localMatrix.m );

    m_flags |= ZP_TRANSFORM_FLAG_DIRTY;
}

void zpTransformComponent::setLocalPositionRotation( const zpVector4fData& position, const zpQuaternion4fData& rotation )
{
    m_localPosition = position;
    m_localRotation = rotation;

    zpVector4f p = zpMath::Vector4Load4( m_localPosition.m );
    zpQuaternion4f r = zpMath::QuaternionLoad4( m_localRotation.m );
    zpVector4f s = zpMath::Vector4Load4( m_localScale.m );

    zpMatrix4f m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, m_localMatrix.m );

    m_flags |= ZP_TRANSFORM_FLAG_DIRTY;
}

void zpTransformComponent::setLocalPositionRotationScale( const zpVector4fData& position, const zpQuaternion4fData& rotation, const zpVector4fData& scale )
{
    m_localPosition = position;
    m_localRotation = rotation;
    m_localScale = scale;

    zpVector4f p = zpMath::Vector4Load4( m_localPosition.m );
    zpQuaternion4f r = zpMath::QuaternionLoad4( m_localRotation.m );
    zpVector4f s = zpMath::Vector4Load4( m_localScale.m );

    zpMatrix4f m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, m_localMatrix.m );

    m_flags |= ZP_TRANSFORM_FLAG_DIRTY;
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

    m_flags |= ZP_TRANSFORM_FLAG_DIRTY;
}

void zpTransformComponent::update( zp_float dt, zp_float rt )
{
    if( ( m_flags & ZP_TRANSFORM_FLAG_DIRTY ) == ZP_TRANSFORM_FLAG_DIRTY )
    {
        zpMatrix4f world = zpMath::MatrixLoad4( m_localMatrix.m );
        zpTransformComponentHandle parent = m_parentTransfrom;

        while( parent.isValid() )
        {
            zpMatrix4f local = zpMath::MatrixLoad4( parent->getLocalMatrix().m );
            world = zpMath::MatrixMul( world, local );
            parent = parent->getParentTransform();
        }

        zpMath::MatrixStore4( world, m_worldMatrix.m );

        m_flags &= ~ZP_TRANSFORM_FLAG_DIRTY;
    }
}

zp_bool zpTransformComponent::isEnabled() const
{
    return ( m_flags & ZP_TRANSFORM_FLAG_ENABLED ) == ZP_TRANSFORM_FLAG_ENABLED;
}

void zpTransformComponent::setEnabled( zp_bool enabled )
{
    if( enabled )
    {
        m_flags |= ZP_TRANSFORM_FLAG_ENABLED;
    }
    else
    {
        m_flags &= ~ZP_TRANSFORM_FLAG_ENABLED;
    }
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

const zpTransformComponent* zpTransformComponentHandle::operator->() const
{
    return isValid() ? &m_transformInstance->transform : ZP_NULL;
}

zpTransformComponent* zpTransformComponentHandle::operator->()
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

void zpTransformComponentManager::createTransformComponent( zpTransformComponentDesc* desc, zpTransformComponentHandle& handle )
{
    createTransformComponent( handle );

    handle->setLocalPositionRotationScale( desc->localPosition, desc->localRotation, desc->localScale );
    handle->setParentObject( desc->parentObject );
    handle->setParentTransform( desc->parentObject->getAllComponents()->transform );
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
