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

#endif // !ZP_OBJECT_MANAGER_H
