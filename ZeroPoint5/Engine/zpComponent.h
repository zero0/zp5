#pragma once
#ifndef ZP_COMPONENT_H
#define ZP_COMPONENT_H

ZP_ABSTRACT_CLASS zpComponent
{
    ZP_NON_COPYABLE( zpComponent );
public:
    zpComponent();
    virtual ~zpComponent();

    zp_hash64 getInstanceId() const;

    void update( zp_float dt, zp_float rt );
    void lateUpdate( zp_float dt, zp_float rt );
    void fixedUpdate( zp_float ft, zp_float rt );

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

    void create();
    void destroy();

    const zpObjectHandle& getParentObject() const;
    zpObjectHandle& getParentObject();

protected:
    void setInstanceId( zp_hash64 instanceId );

    virtual void onEnabled() {}
    virtual void onDisabled() {}

    virtual void onCreated() {}
    virtual void onDestroyed() {}

    virtual void onUpdate( zp_float dt, zp_float rt ) {}
    virtual void onLateUpdate( zp_float dt, zp_float rt ) {}
    virtual void onFixedUpdate( zp_float ft, zp_float rt ) {}

private:
    zpObjectHandle m_parentObject;
    zp_hash64 m_instanceId;

    zp_ulong m_flags;
};

template< typename T >
struct zpComponentInstance
{
    T m_component;
    zp_size_t m_refCount;
};

template< typename Component, typename Instance, typename Manager >
class zpComponentHandle
{
public:
    typedef zpComponentHandle< Component, Instance, Manager > Handle;

    zpComponentHandle();
    zpComponentHandle( const Handle& other );
    zpComponentHandle( Handle&& other );
    ~zpComponentHandle();

    Handle& operator=( const Handle& other );
    Handle& operator=( Handle&& other );

    const Component* operator->() const;
    Component* operator->();

    zp_bool isValid() const;

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, Instance* objectInstance );
    void invalidate();

    zp_hash64 m_instanceId;
    Instance* m_instance;

    friend Manager;
};

#endif // !ZP_COMPONENT_H
