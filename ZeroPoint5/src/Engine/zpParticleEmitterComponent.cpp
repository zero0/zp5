#include "Engine/zpEngine.h"
//#include <new>

enum : zp_size_t
{
    ZP_MESH_RENDERER_FLAG_ENABLED,
    ZP_MESH_RENDERER_FLAG_DIRTY,
};

zpParticleEmitterComponent::zpParticleEmitterComponent()
    : zpComponent()
{
}

zpParticleEmitterComponent::~zpParticleEmitterComponent()
{
}

void zpParticleEmitterComponent::onUpdate( zp_float dt, zp_float rt )
{
    if( isFlagSet( ZP_COMPONENT_FLAG_DIRTY ) )
    {

        clearFlag( ZP_COMPONENT_FLAG_DIRTY );
    }
}

//
//
//

zpParticleEmitterComponentManager::zpParticleEmitterComponentManager()
{
}

zpParticleEmitterComponentManager::~zpParticleEmitterComponentManager()
{
}

void zpParticleEmitterComponentManager::createParticleEmitterComponent( handle_reference handle, zpParticleEmitterComponentDesc* desc )
{
    createComponent( handle );

    if( desc )
    {
    }
}
