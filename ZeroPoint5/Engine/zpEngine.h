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

#include "zpObject.h"
#include "zpComponent.h"
#include "zpBaseApplication.h"
#include "zpMain.h"
#include "zpTransformComponent.h"

#endif // !ZP_ENGINE_H
