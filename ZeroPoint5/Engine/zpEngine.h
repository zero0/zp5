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


#include "zpObjectManager.h"

#include "zpComponent.h"

#include "zpTransformComponent.h"

#include "zpAllComponents.h"

#include "zpObject.h"
#include "zpScene.h"
#include "zpBaseApplication.h"
#include "zpMain.h"

#endif // !ZP_ENGINE_H
