#pragma once
#ifndef ZP_OBJECT_H
#define ZP_OBJECT_H

class zpObject
{
public:
    zpObject();
    ~zpObject();

    zp_hash64 getInstanceId() const;

private:
    void setInstanceId( zp_hash64 instanceId );

    zp_hash64 m_instanceId;

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

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpObjectInstance* objectInstance );
    void invalidate();

    zp_hash64 m_instanceId;
    zpObjectInstance* m_objectInstance;

    friend class zpObjectManager;
};

class zpObjectManager
{
public:
    zpObjectManager();
    ~zpObjectManager();

    void createObject( zpObjectHandle& objHandle );
    void destryObject( zpObjectHandle& objHandle );

    void update();

    void garbageCollect();

private:
    zpBlockAllocator m_objectMemory;
    zpVector< zpObjectInstance* > m_activeObjects;

    zp_hash64 m_newObjectInstanceId;
};

#endif // !ZP_OBJECT_H
