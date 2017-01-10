#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"

enum zpTopology : zp_uint
{
    ZP_TOPOLOGY_UNKNOWN = 0,

    ZP_TOPOLOGY_POINT_LIST,

    ZP_TOPOLOGY_LINE_LIST,
    ZP_TOPOLOGY_LINE_STRIP,

    ZP_TOPOLOGY_TRIANGLE_LIST,
    ZP_TOPOLOGY_TRIANGLE_STRIP,

    zpTopology_Count,
    zpTopology_Force32 = ZP_FORECE_32BIT
};

enum zpRenderingCommandType : zp_uint
{
    ZP_RENDERING_COMMNAD_NOOP,

    ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT,

    ZP_RENDERING_COMMNAD_CLEAR,

    ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE,
    ZP_RENDERING_COMMNAD_DRAW_BUFFERED,

    ZP_RENDERING_COMMNAD_DRAW_INSTANCED,

    zpRenderingCommandType_Count,
    zpRenderingCommandType_Force32 = ZP_FORECE_32BIT
};

enum zpVertexFormat : zp_uint
{
    ZP_VERTEX_FORMAT_NONE,

    ZP_VERTEX_FORMAT_VERTEX_COLOR,
    ZP_VERTEX_FORMAT_VERTEX_UV,
    ZP_VERTEX_FORMAT_VERTEX_COLOR_UV,
    ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV,
    ZP_VERTEX_FORMAT_VERTEX_NORMAL_UV2,
    ZP_VERTEX_FORMAT_VERTEX_NORMAL_COLOR_UV,
    ZP_VERTEX_FORMAT_VERTEX_NORMAL_TANGENT_COLOR_UV,
    ZP_VERTEX_FORMAT_VERTEX_NORMAL_TANGENT_COLOR_UV2,

    zpVertexFormat_Count,
    zpVertexFormat_Force32 = ZP_FORECE_32BIT
};

struct zpViewport
{
    zp_float width;
    zp_float height;
    zp_float minDepth;
    zp_float maxDepth;
    zp_float topX;
    zp_float topY;
};

union zpRenderingSortKey
{
    struct
    {
        zp_byte anchor : 1;
        zp_byte viewport : 3;
        zp_byte transparent : 1;
        zp_byte layer : 3;
        zp_uint depth : 24;
        zp_uint material : 30;
        zp_uint pass : 2;
    };
    zp_ulong key;
};

struct zpRenderingCommand
{
    zpRenderingCommandType type;
    zpRenderingSortKey sortKey;

    union
    {
        struct // draw command data
        {
            zpTopology topology;

            zp_size_t vertexOffset;
            zp_size_t vertexCount;
            zp_size_t indexOffset;
            zp_size_t indexCount;

            zpMatrix4f transform;
        };

        struct // draw instanced data
        {
            zp_hash64 instanceId;

            zpMatrix4f instanceTransform;
        };

        struct // scissor rect data
        {
            zpRecti scissorRect;
        };

        struct // clear data
        {
            zpColor clearColor;
            zp_float clearDepth;
            zp_uint clearStencil;
        };
    };
};

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

#include "zpRenderingContext.h"
#include "zpRenderingEngine.h"

#endif // !ZP_RENDERING_H
