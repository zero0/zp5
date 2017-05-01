#pragma once
#ifndef ZP_ENGINE_H
#define ZP_ENGINE_H

#include "Core\zpCore.h"
#include "Rendering\zpRendering.h"

typedef void*(PlatformAllocateFunc)( zp_size_t size );
typedef void(PlatformFreeFunc)( void* ptr );

struct zpPlatform
{
    PlatformAllocateFunc allocate;
    PlatformFreeFunc free;
};

enum : zp_hash64
{
    ZP_HANDLE_ID_EMPTY = 0ULL,
    ZP_HANDLE_ID_INVALID = static_cast<zp_hash64>( -1 )
};

#include "zpInput.h"

#include "zpObjectManager.h"

#include "zpComponent.h"

#include "zpTransformComponent.h"
#include "zpParticleEmitterComponent.h"
#include "zpMeshRendererComponent.h"

#include "zpAllComponents.h"

#include "zpObject.h"
#include "zpScene.h"
#include "zpBaseApplication.h"
#include "zpMain.h"

#include "zpDebug.h"

#endif // !ZP_ENGINE_H
