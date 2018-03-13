#pragma once
#ifndef ZP_TRANSFORM_COMPONENT_H
#define ZP_TRANSFORM_COMPONENT_H

class zpTransformComponent;
struct zpTransformComponentInstance;

class zpTransformComponentHandle
{
public:
    zpTransformComponentHandle();
    zpTransformComponentHandle( const zpTransformComponentHandle& other );
    zpTransformComponentHandle( zpTransformComponentHandle&& other );
    ~zpTransformComponentHandle();

    zpTransformComponentHandle& operator=( const zpTransformComponentHandle& other );
    zpTransformComponentHandle& operator=( zpTransformComponentHandle&& other );

    ZP_FORCE_INLINE const zpTransformComponent* operator->() const
    {
        return get();
    }
    ZP_FORCE_INLINE zpTransformComponent* operator->()
    {
        return get();
    }

    const zpTransformComponent* get() const;
    zpTransformComponent* get();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpTransformComponentInstance* objectInstance );

    zp_hash64 m_instanceId;
    zpTransformComponentInstance* m_transformInstance;

    friend class zpTransformComponentManager;
};

class zpTransformComponent
{
public:
    zpTransformComponent();
    ~zpTransformComponent();

    void setParentObject( const zpObjectHandle& parent );
    const zpObjectHandle& getParentObject() const;

    void setLocalPosition( const zpVector4fData& position );
    void setLocalPositionRotation( const zpVector4fData& position, const zpQuaternion4fData& rotation );
    void setLocalPositionRotationScale( const zpVector4fData& position, const zpQuaternion4fData& rotation, const zpVector4fData& scale );

    const zpVector4fData& getLocalPosition() const;
    const zpQuaternion4fData& getLocalRotation() const;
    const zpVector4fData& getLocalScale() const;

    const zpMatrix4fData& getLocalMatrix() const;
    const zpMatrix4fData& getWorldMatrix() const;

    const zpTransformComponentHandle& getParentTransform() const;
    zpTransformComponentHandle& getParentTransform();
    void setParentTransform( const zpTransformComponentHandle& parent );

    void update( zp_float dt, zp_float rt );

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

    zp_hash64 getInstanceId() const;

private:
    void setInstanceId( zp_hash64 instanceId );

    zpVector4fData m_localPosition;
    zpQuaternion4fData m_localRotation;
    zpVector4fData m_localScale;

    zpMatrix4fData m_localMatrix;
    zpMatrix4fData m_worldMatrix;

    zp_hash64 m_instanceID;
    zp_ulong m_flags;

    zpTransformComponentHandle m_parentTransfrom;
    zpObjectHandle m_parentObject;

    friend class zpTransformComponentManager;
};

struct zpTransformComponentInstance
{
    zpTransformComponent transform;
    zp_size_t refCount;
};

struct zpTransformComponentDesc
{
    zpVector4fData localPosition;
    zpQuaternion4fData localRotation;
    zpVector4fData localScale;

    zpObjectHandle parentObject;
};

class zpTransformComponentManager
{
public:
    zpTransformComponentManager();
    ~zpTransformComponentManager();

    void update( zp_float dt, zp_float rt );

    void createTransformComponent( zpTransformComponentHandle& handle );
    void createTransformComponent( zpTransformComponentHandle& handle, zpTransformComponentDesc* desc );

    void garbageCollect();

    zp_bool findComponentByID( zp_hash64 instanceId, zpTransformComponentHandle& handle ) const;

private:
    zpVector< zpTransformComponentInstance* > m_activeComponents;
    zp_size_t m_newTransformComponentInstanceId;
};

//
// RectTransform
//

struct zpRectTransformComponentDesc
{
    zpRectf rect;
};

class zpRectTransformComponentManager;
class zpRectTransformComponent;
typedef zpComponentInstance< zpRectTransformComponent > zpRectTransformComponentInstance;
typedef zpComponentHandle< zpRectTransformComponent, zpRectTransformComponentInstance, zpRectTransformComponentManager > zpRectTransformComponentHandle;

class zpRectTransformComponentManager : public zpComponentManager< zpRectTransformComponent, zpRectTransformComponentInstance, zpRectTransformComponentManager, zpRectTransformComponentHandle > {
public:
    void createRectTransformComponent( handle_reference h, zpRectTransformComponentDesc* desc );

protected:

};

class zpRectTransformComponent : public zpComponent< zpRectTransformComponentManager >
{
protected:

};

#endif // !ZP_TRANSFORM_COMPONENT_H
