#include "Engine/zpEngine.h"
//#include <new>

enum : zp_size_t
{
    ZP_TRANSFORM_FLAG_ENABLED,
    ZP_TRANSFORM_FLAG_DIRTY,
};

zpTransformComponent::zpTransformComponent()
    : zpComponent()
    , m_localPosition()
    , m_localRotation()
    , m_localScale()
    , m_localMatrix()
    , m_worldMatrix()
    , m_parentTransfrom()
{
    setLocalPositionRotationScale( { 0, 0, 0, 1 }, { 0, 0, 0, 1 }, { 1, 1, 1, 0 } );
}

zpTransformComponent::~zpTransformComponent()
{
}

void zpTransformComponent::setLocalPosition( const zpVector4f& position )
{
    m_localPosition = position;

    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpTransformComponent::setLocalRotation( const zpQuaternion4f& rotation )
{
    m_localRotation = rotation;

    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpTransformComponent::setLocalScale( const zpVector4f& scale )
{
    m_localScale = scale;

    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpTransformComponent::setLocalPositionRotation( const zpVector4f& position, const zpQuaternion4f& rotation )
{
    m_localPosition = position;
    m_localRotation = rotation;

    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpTransformComponent::setLocalPositionRotationScale( const zpVector4f& position, const zpQuaternion4f& rotation, const zpVector4f& scale )
{
    m_localPosition = position;
    m_localRotation = rotation;
    m_localScale = scale;

    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

const zpVector4f& zpTransformComponent::getLocalPosition() const
{
    return m_localPosition;
}

const zpQuaternion4f& zpTransformComponent::getLocalRotation() const
{
    return m_localRotation;
}

const zpVector4f& zpTransformComponent::getLocalScale() const
{
    return m_localScale;
}

const zpMatrix4f& zpTransformComponent::getLocalMatrix() const
{
    return m_localMatrix;
}

const zpMatrix4f& zpTransformComponent::getWorldMatrix() const
{
    return m_worldMatrix;
}

const zpVector4f zpTransformComponent::getWorldPosition() const
{
    zpVector4f wp = m_localPosition;
    return wp;
}

const zpQuaternion4f zpTransformComponent::getWorldRotation() const
{
    zpQuaternion4f wr = m_localRotation;
    return wr;
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

    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpTransformComponent::onUpdate( zp_float dt, zp_float rt )
{
    if( isFlagSet( ZP_COMPONENT_FLAG_DIRTY ) )
    {
        updateLocalMatrix();

        updateWorldMatrix();

        clearFlag( ZP_COMPONENT_FLAG_DIRTY );
    }
}

ZP_INLINE void zpTransformComponent::updateLocalMatrix()
{
    ZP_ALIGN16 zpVector4f pos = m_localPosition;
    ZP_ALIGN16 zpQuaternion4f rot = m_localRotation;
    ZP_ALIGN16 zpVector4f scl = m_localScale;

    zpVector4fSimd p = zpMath::Vector4Load4( pos.m );
    zpQuaternion4fSimd r = zpMath::QuaternionLoad4( rot.m );
    zpVector4fSimd s = zpMath::Vector4Load4( scl.m );

    ZP_ALIGN16 zpMatrix4f mat;
    zpMatrix4fSimd m = zpMath::MatrixTRS( p, r, s );
    zpMath::MatrixStore4( m, mat.m );
    m_localMatrix = mat;
}

ZP_INLINE void zpTransformComponent::updateWorldMatrix()
{
    ZP_ALIGN16 zpMatrix4f mat = m_localMatrix;

    zpMatrix4fSimd world = zpMath::MatrixLoad4( mat.m );
    zpTransformComponentHandle parent = m_parentTransfrom;

    while( parent.isValid() )
    {
        zpMatrix4fSimd local = zpMath::MatrixLoad4( parent->getLocalMatrix().m );
        world = zpMath::MatrixMul( world, local );
        parent = parent->getParentTransform();
    }

    zpMath::MatrixStore4( world, mat.m );

    m_worldMatrix = mat;
}

//
//
//

zpTransformComponentManager::zpTransformComponentManager()
{
}

zpTransformComponentManager::~zpTransformComponentManager()
{
}

void zpTransformComponentManager::createTransformComponent( handle_reference handle, zpTransformComponentDesc* desc )
{
    createComponent( handle );

    if( desc )
    {
        handle->setLocalPositionRotationScale( desc->localPosition, desc->localRotation, desc->localScale );
        handle->setParentObject( desc->parentObject );
        handle->setParentTransform( desc->parentObject->getAllComponents()->transform );
    }
}
