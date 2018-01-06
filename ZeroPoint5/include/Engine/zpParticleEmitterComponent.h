#pragma once
#ifndef ZP_PARTICLE_EMITTER_COMPONENT_H
#define ZP_PARTICLE_EMITTER_COMPONENT_H

class zpParticleEmitterComponent;
struct zpParticleEmitterComponentInstance;

enum zpParticleEffectShape
{
    ZP_PARTICLE_EFFECT_SHAPE_SPHERE,
    ZP_PARTICLE_EFFECT_SHAPE_HEMISPHERE,
    ZP_PARTICLE_EFFECT_SHAPE_CONE,
    ZP_PARTICLE_EFFECT_SHAPE_BOX,

    zpParticleEffectShape_Count,
    zpParticleEffectShape_Force32 = ZP_FORECE_32BIT
};

enum zpParticleEffectRange
{
    ZP_PARTICLE_EFFECT_RANGE_CONSTANT,
    ZP_PARTICLE_EFFECT_RANGE_RANDOM,
    ZP_PARTICLE_EFFECT_RANGE_LIFETIME,
    ZP_PARTICLE_EFFECT_RANGE_VELOCITY,

    zpParticleEffectRange_Count,
    zpParticleEffectRange_Force32 = ZP_FORECE_32BIT
};

enum zpParticleEffectState
{
    ZP_PARTICLE_EFFECT_STATE_DISABLED,
    ZP_PARTICLE_EFFECT_STATE_STARTING,
    ZP_PARTICLE_EFFECT_STATE_PLAYING,
    ZP_PARTICLE_EFFECT_STATE_FINISHING,

    zpParticleEffectState_Count,
    zpParticleEffectState_Force32 = ZP_FORECE_32BIT
};

enum zpParticleEffectFlag
{
    ZP_PARTICLE_EFFECT_FLAG_ENABLED,
    ZP_PARTICLE_EFFECT_FLAG_EMIT_FROM_SHELL,
    ZP_PARTICLE_EFFECT_FLAG_RANDOM_DIRECTION,
    ZP_PARTICLE_EFFECT_FLAG_LOOPING,
    ZP_PARTICLE_EFFECT_FLAG_PRE_WARM,
    ZP_PARTICLE_EFFECT_FLAG_USE_REAL_TIME,
    ZP_PARTICLE_EFFECT_FLAG_WORLD_SPACE,
    ZP_PARTICLE_EFFECT_FLAG_IS_BILLBOARD,
    ZP_PARTICLE_EFFECT_FLAG_PLAY_ON_CREATE,
    ZP_PARTICLE_EFFECT_FLAG_USE_DEPTH_SORT,

    zpParticleEffectFlag_Count,
    zpParticleEffectFlag_Force32 = ZP_FORECE_32BIT
};

struct zpParticle
{
    zpVector4fData position;
    zpVector4fData velocity;
    zpVector4fData rotation;
    zpVector4fData angularVelocity;
    zpVector4fData scale;
    zpVector4fData normal;
    zpColorf color;
    zp_float life;
    zp_float progress;
};

struct zpParticleEffectPart
{
    zpVector4fData scale;
    zpColorf color;

    zp_float speed;
    zp_float life;
    zp_float emitRate;
};

struct zpParticleEffect
{
    zpString name;

    zp_float delay;
    zp_float duration;
    zp_float time;
    zp_float emitTime;

    zp_float flipbookSpeed;
    zp_uint flipbookX;
    zp_uint flipbookY;
    zpParticleEffectShape shape;

    zpVector4fData gravity;
    zpVector4fData shapeSize;

    zpParticleEffectRange scaleRange;
    zpParticleEffectRange colorRange;
    zpParticleEffectRange speedRange;
    zpParticleEffectRange lifeRange;
    zpParticleEffectRange emitRateRange;

    zpParticleEffectPart minPart;
    zpParticleEffectPart maxPart;

    zpParticleEffectState state;
    zp_ulong m_flags;

    zp_size_t particleOffset;
    zp_size_t particleLength;
    zp_size_t maxParticleCount;

    zpMaterialHandle material;
};

class zpParticleEmitterComponentHandle
{
public:
    zpParticleEmitterComponentHandle();
    zpParticleEmitterComponentHandle( const zpParticleEmitterComponentHandle& other );
    zpParticleEmitterComponentHandle( zpParticleEmitterComponentHandle&& other );
    ~zpParticleEmitterComponentHandle();

    zpParticleEmitterComponentHandle& operator=( const zpParticleEmitterComponentHandle& other );
    zpParticleEmitterComponentHandle& operator=( zpParticleEmitterComponentHandle&& other );

    ZP_FORCE_INLINE const zpParticleEmitterComponent* operator->() const
    {
        return get();
    }
    ZP_FORCE_INLINE zpParticleEmitterComponent* operator->()
    {
        return get();
    }

    const zpParticleEmitterComponent* get() const;
    zpParticleEmitterComponent* get();

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

    zp_size_t addEffect( zpParticleEffect* effect );
    void removeEffect( const zp_char* effectName );

    zp_size_t getEffectIndex( const zp_char* effectName ) const;

    void play( zp_size_t effectIndex, zp_bool force = false );
    void stop( zp_size_t effectIndex );

    void playAll( zp_bool force = false );
    void stopAll();

    zp_bool isPlaying( zp_size_t effectIndex ) const;
    zp_bool isAllPlaying() const;
    zp_bool isAnyPlaying() const;

private:
    void setInstanceId( zp_hash64 instanceId );

    zp_hash64 m_instanceID;
    zp_ulong m_flags;

    zpObjectHandle m_parentObject;

    zpVector< zpParticleEffect > m_effects;
    zpVector< zpParticle > m_particles;

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
