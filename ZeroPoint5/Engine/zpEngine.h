#pragma once
#ifndef ZP_ENGINE_H
#define ZP_ENGINE_H

#include "Core\zpCore.h"

typedef void*(PlatformAllocateFunc)( zp_size_t size );
typedef void(PlatformFreeFunc)( void* ptr );

struct zpPlatform
{
    PlatformAllocateFunc allocate;
    PlatformFreeFunc free;
};

#include "zpBaseApplication.h"
#include "zpMain.h"

#endif // !ZP_ENGINE_H
