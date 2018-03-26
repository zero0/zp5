#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

struct zpTransformComponentDesc
{
    zpVector4fData localPosition;
    zpQuaternion4fData localRotation;
    zpVector4fData localScale;

    zpObjectHandle parentObject;
};

class zpTransformComponentManager;
class zpTransformComponent;

typedef zpComponentInstance< zpTransformComponent > zpTransformComponentInstance;
typedef zpComponentHandle< zpTransformComponent, zpTransformComponentInstance, zpTransformComponentManager > zpTransformComponentHandle;


class zpTransformComponentManager : public zpComponentManager< zpTransformComponent, zpTransformComponentInstance, zpTransformComponentManager, zpTransformComponentHandle >
{
public:
    zpTransformComponentManager();
    ~zpTransformComponentManager();

    void createTransformComponent( handle_reference h, zpTransformComponentDesc* desc );

protected:

};

class zpTransformComponent : public zpComponent< zpTransformComponentManager >
{
public:
    zpTransformComponent();
    ~zpTransformComponent();

    void setLocalPosition( const zpVector4fData& position );
    void setLocalRotation( const zpQuaternion4fData& rotation );
    void setLocalScale( const zpVector4fData& scale );

    void setLocalPositionRotation( const zpVector4fData& position, const zpQuaternion4fData& rotation );
    void setLocalPositionRotationScale( const zpVector4fData& position, const zpQuaternion4fData& rotation, const zpVector4fData& scale );

    const zpVector4fData& getLocalPosition() const;
    const zpQuaternion4fData& getLocalRotation() const;
    const zpVector4fData& getLocalScale() const;

    const zpMatrix4fData& getLocalMatrix() const;
    const zpMatrix4fData& getWorldMatrix() const;

    const zpVector4fData getWorldPosition() const;
    const zpQuaternion4fData getWorldRotation() const;

    const zpTransformComponentHandle& getParentTransform() const;
    zpTransformComponentHandle& getParentTransform();
    void setParentTransform( const zpTransformComponentHandle& parent );

protected:
    void onUpdate( zp_float dt, zp_float rt );

private:
    ZP_INLINE void updateLocalMatrix();
    ZP_INLINE void updateWorldMatrix();

    zpTransformComponentHandle m_parentTransfrom;

    zpVector4fData m_localPosition;
    zpQuaternion4fData m_localRotation;
    zpVector4fData m_localScale;

    zpMatrix4fData m_localMatrix;
    zpMatrix4fData m_worldMatrix;

};



#endif // !ZP_TRANSFORM_COMPONENT_H
