#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

class zpTransfromComponent;
class zpTransformComponentManager;

typedef zpComponentInstance< zpTransfromComponent > zpTransformComponentInstance;
typedef zpComponentHandle< zpTransfromComponent, zpTransformComponentInstance, zpTransformComponentManager > zpTransformComponentHandle;

class zpTransfromComponent
{
public:
    zpTransfromComponent();
    ~zpTransfromComponent();

    void setLocalPosition( zpVector4fParamF position );
    void setLocalPositionRotation( zpVector4fParamF position, zpQuaternion4f rotation );
    void setLocalPositionRotationScale( zpVector4fParamF position, zpQuaternion4f rotation, zpVector4f scale );

    zpVector4f getLocalPosition() const;
    zpQuaternion4f getLocalRotation() const;
    zpVector4f getLocalScale() const;

    zpMatrix4f getLocalMatrix() const;
    zpMatrix4f getWorldMatrix() const;

    const zpTransformComponentHandle& getParentTransform() const;
    zpTransformComponentHandle& getParentTransform();
    void setParentTransform( const zpTransformComponentHandle& parent );

    void update( zp_float dt, zp_float rt );

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

private:
    zpVector4f m_localPosition;
    zpQuaternion4f m_localRotation;
    zpVector4f m_localScale;

    zpMatrix4f m_localMatrix;
    zpMatrix4f m_worldMatrix;

    zp_hash64 m_instanceID;
    zp_ulong m_flags;

    zpTransformComponentHandle m_parentTransfrom;
    zpObjectHandle m_parentObject;
};


class zpTransformComponentManager
{
public:
    zpTransformComponentManager();
    ~zpTransformComponentManager();

    void update( zp_float dt, zp_float rt );

    void create( zpTransformComponentHandle& handle );

    void garbageCollect();

    zp_bool findComponentByID( zp_hash64 instanceId, zpTransformComponentHandle& handle ) const;

private:
    zpVector< zpTransformComponentInstance* > m_activeComponents;
};

#endif // !ZP_TRANSFORM_COMPONENT_H
