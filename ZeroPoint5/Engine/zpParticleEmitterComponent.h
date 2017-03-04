#pragma once
#ifndef ZP_PARTICLE_EMITTER_COMPONENT_H
#define ZP_PARTICLE_EMITTER_COMPONENT_H

class zpParticleEmitterComponent;
struct zpParticleEmitterComponentInstance;

class zpParticleEmitterComponentHandle
{
public:
    zpParticleEmitterComponentHandle();
    zpParticleEmitterComponentHandle( const zpParticleEmitterComponentHandle& other );
    zpParticleEmitterComponentHandle( zpParticleEmitterComponentHandle&& other );
    ~zpParticleEmitterComponentHandle();

    zpParticleEmitterComponentHandle& operator=( const zpParticleEmitterComponentHandle& other );
    zpParticleEmitterComponentHandle& operator=( zpParticleEmitterComponentHandle&& other );

    const zpParticleEmitterComponent* operator->() const;
    zpParticleEmitterComponent* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpParticleEmitterComponentInstance* objectInstance );

    zp_hash64 m_instanceId;
    zpParticleEmitterComponentInstance* m_particleEmitterInstance;

    friend class zpParticleEmitterComponentManager;
};

class zpParticleEmitterComponent
{
public:
    zpParticleEmitterComponent();
    ~zpParticleEmitterComponent();

    void setParentObject( const zpObjectHandle& parent );
    const zpObjectHandle& getParentObject() const;

    void update( zp_float dt, zp_float rt );

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

    zp_hash64 getInstanceId() const;

private:
    void setInstanceId( zp_hash64 instanceId );

    zp_hash64 m_instanceID;
    zp_ulong m_flags;

    zpObjectHandle m_parentObject;

    friend class zpParticleEmitterComponentManager;
};

struct zpParticleEmitterComponentInstance
{
    zpParticleEmitterComponent particleEmitter;
    zp_size_t refCount;
};

struct zpParticleEmitterComponentDesc
{
    zpObjectHandle parentObject;
};

class zpParticleEmitterComponentManager
{
public:
    zpParticleEmitterComponentManager();
    ~zpParticleEmitterComponentManager();

    void update( zp_float dt, zp_float rt );

    void createParticleEmitterComponent( zpParticleEmitterComponentHandle& handle );
    void createParticleEmitterComponent( zpParticleEmitterComponentHandle& handle, zpParticleEmitterComponentDesc* desc );

    void garbageCollect();

    zp_bool findComponentByID( zp_hash64 instanceId, zpParticleEmitterComponentHandle& handle ) const;

private:
    zpVector< zpParticleEmitterComponentInstance* > m_activeComponents;
    zp_size_t m_newParticleEmitterComponentInstanceId;
};

#endif // ZP_PARTICLE_EMITTER_COMPONENT_H
