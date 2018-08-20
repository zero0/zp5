#pragma once
#ifndef ZP_PARTICLE_EMITTER_COMPONENT_H
#define ZP_PARTICLE_EMITTER_COMPONENT_H

struct zpParticleEmitterComponentDesc
{
    zpObjectHandle parentObject;
};

class zpParticleEmitterComponentManager;
class zpParticleEmitterComponent;

typedef zpHandleInstance< zpParticleEmitterComponent > zpParticleEmitterComponentInstance;
typedef zpHandle< zpParticleEmitterComponent > zpParticleEmitterComponentHandle;

enum zpParticleEffectShape
{
    ZP_PARTICLE_EFFECT_SHAPE_SPHERE,
    ZP_PARTICLE_EFFECT_SHAPE_HEMISPHERE,
    ZP_PARTICLE_EFFECT_SHAPE_CONE,
    ZP_PARTICLE_EFFECT_SHAPE_BOX,

    zpParticleEffectShape_Count,
    zpParticleEffectShape_Force32 = ZP_FORCE_32BIT
};

enum zpParticleEffectRange
{
    ZP_PARTICLE_EFFECT_RANGE_CONSTANT,
    ZP_PARTICLE_EFFECT_RANGE_RANDOM,
    ZP_PARTICLE_EFFECT_RANGE_LIFETIME,
    ZP_PARTICLE_EFFECT_RANGE_VELOCITY,

    zpParticleEffectRange_Count,
    zpParticleEffectRange_Force32 = ZP_FORCE_32BIT
};

enum zpParticleEffectState
{
    ZP_PARTICLE_EFFECT_STATE_DISABLED,
    ZP_PARTICLE_EFFECT_STATE_STARTING,
    ZP_PARTICLE_EFFECT_STATE_PLAYING,
    ZP_PARTICLE_EFFECT_STATE_FINISHING,

    zpParticleEffectState_Count,
    zpParticleEffectState_Force32 = ZP_FORCE_32BIT
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
    zpParticleEffectFlag_Force32 = ZP_FORCE_32BIT
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

//
//
//

class zpParticleEmitterComponentManager : public zpComponentManager< zpParticleEmitterComponent >
{
public:
    zpParticleEmitterComponentManager();
    ~zpParticleEmitterComponentManager();

    void createParticleEmitterComponent( handle_reference h, zpParticleEmitterComponentDesc* desc );

protected:
};

//
//
//

class zpParticleEmitterComponent : public zpComponent
{
public:
    zpParticleEmitterComponent();
    ~zpParticleEmitterComponent();

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

protected:
    void onUpdate( zp_float dt, zp_float rt );

private:
    zpVector< zpParticleEffect > m_effects;
    zpVector< zpParticle > m_particles;
};

#endif // ZP_PARTICLE_EMITTER_COMPONENT_H
