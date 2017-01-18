#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

class zpObject
{
public:
    zpObject();
    ~zpObject();

    zp_hash64 getInstanceId() const;

    const zpString& getName() const;
    void setName( const zp_char* name );

    zp_ulong getLayers() const;
    void setLayers( zp_ulong layers );
    void markOnLayer( zp_int layerIndex, zp_bool onLayer );
    zp_bool isOnLayer( zp_int layerIndex ) const;

    zp_ulong getTags() const;
    void setTags( zp_ulong tags );
    void markOnTag( zp_int tagIndex, zp_bool tagged );
    zp_bool hasTag( zp_int tagIndex ) const;

private:
    void setInstanceId( zp_hash64 instanceId );

    zp_hash64 m_instanceId;

    zp_ulong m_layers;
    zp_ulong m_tags;

    zpString m_name;

    friend class zpObjectManager;
};

struct zpObjectInstance
{
    zpObject m_object;
    zp_size_t m_refCount;
};

class zpObjectHandle
{
public:
    zpObjectHandle();
    zpObjectHandle( const zpObjectHandle& other );
    zpObjectHandle( zpObjectHandle&& other );
    ~zpObjectHandle();

    zpObjectHandle& operator=( const zpObjectHandle& other );
    zpObjectHandle& operator=( zpObjectHandle&& other );

    const zpObject* operator->() const;
    zpObject* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpObjectInstance* objectInstance );

    zp_hash64 m_instanceId;
    zpObjectInstance* m_objectInstance;

    friend class zpObjectManager;
};

class zpObjectManager
{
public:
    zpObjectManager();
    ~zpObjectManager();

    void createObject( zpObjectHandle& handle );

    zp_bool findObjectByID( zp_hash64 instanceId, zpObjectHandle& handle ) const;

    void update();

    void garbageCollect();

private:
    zpVector< zpObjectInstance* > m_activeObjects;

    zp_hash64 m_newObjectInstanceId;
};

#endif // !ZP_OBJECT_H
