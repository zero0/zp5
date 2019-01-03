#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

struct zpTransformComponentDesc
{
    zpVector4f localPosition;
    zpQuaternion4f localRotation;
    zpVector4f localScale;

    zpObjectHandle parentObject;
};

class zpTransformComponentManager;
class zpTransformComponent;

typedef zpHandleInstance< zpTransformComponent > zpTransformComponentInstance;
typedef zpHandle< zpTransformComponent > zpTransformComponentHandle;

//
//
//

class zpTransformComponentManager : public zpComponentManager< zpTransformComponent >
{
public:
    zpTransformComponentManager();
    ~zpTransformComponentManager();

    void createTransformComponent( handle_reference h, zpTransformComponentDesc* desc );

protected:

};

//
//
//

class zpTransformComponent : public zpComponent
{
public:
    zpTransformComponent();
    ~zpTransformComponent();

    void setLocalPosition( const zpVector4f& position );
    void setLocalRotation( const zpQuaternion4f& rotation );
    void setLocalScale( const zpVector4f& scale );

    void setLocalPositionRotation( const zpVector4f& position, const zpQuaternion4f& rotation );
    void setLocalPositionRotationScale( const zpVector4f& position, const zpQuaternion4f& rotation, const zpVector4f& scale );

    const zpVector4f& getLocalPosition() const;
    const zpQuaternion4f& getLocalRotation() const;
    const zpVector4f& getLocalScale() const;

    const zpMatrix4f& getLocalMatrix() const;
    const zpMatrix4f& getWorldMatrix() const;

    const zpVector4f getWorldPosition() const;
    const zpQuaternion4f getWorldRotation() const;

    const zpTransformComponentHandle& getParentTransform() const;
    zpTransformComponentHandle& getParentTransform();
    void setParentTransform( const zpTransformComponentHandle& parent );

protected:
    void onUpdate( zp_float dt, zp_float rt );

private:
    ZP_INLINE void updateLocalMatrix();
    ZP_INLINE void updateWorldMatrix();

    zpTransformComponentHandle m_parentTransfrom;

    zpVector4f m_localPosition;
    zpQuaternion4f m_localRotation;
    zpVector4f m_localScale;

    zpMatrix4f m_localMatrix;
    zpMatrix4f m_worldMatrix;
};

#endif // !ZP_TRANSFORM_COMPONENT_H
