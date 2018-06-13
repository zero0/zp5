#pragma once
#ifndef ZP_OBJECT_MANAGER_H
#define ZP_OBJECT_MANAGER_H

class zpObject;
struct zpObjectInstance;

class zpObjectHandle
{
public:
    zpObjectHandle();
    zpObjectHandle( const zpObjectHandle& other );
    zpObjectHandle( zpObjectHandle&& other );
    ~zpObjectHandle();

    zpObjectHandle& operator=( const zpObjectHandle& other );
    zpObjectHandle& operator=( zpObjectHandle&& other );

    ZP_FORCE_INLINE const zpObject* operator->() const
    {
        return get();
    }
    ZP_FORCE_INLINE zpObject* operator->()
    {
        return get();
    }

    const zpObject* get() const;
    zpObject* get();

    zp_bool isValid() const;
    void release();

    zp_bool operator==( const zpObjectHandle& other ) const;

    ZP_FORCE_INLINE operator zp_bool() const
    {
        return isValid();
    }

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

    void setup();
    void teardown();

    void createObject( zpObjectHandle& handle );
    void destroyObject( zpObjectHandle& handle );

    zp_bool findObjectByID( zp_hash64 instanceId, zpObjectHandle& handle ) const;

    void update();

    void garbageCollect();

    const zpObjectInstance* const* beginActiveObjects() const;
    const zpObjectInstance* const* endActiveObjects() const;

private:
    zpVector< zpObjectInstance* > m_activeObjects;

    zp_hash64 m_newObjectInstanceId;
};

#endif // !ZP_OBJECT_MANAGER_H
