#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"

enum zpDisplayFormat : zp_uint
{
    ZP_DISPLAY_FORMAT_UNKNOWN = 0,

    // R Component
    ZP_DISPLAY_FORMAT_R8_UINT,
    ZP_DISPLAY_FORMAT_R8_SINT,

    ZP_DISPLAY_FORMAT_R16_UINT,
    ZP_DISPLAY_FORMAT_R16_SINT,
    ZP_DISPLAY_FORMAT_R16_FLOAT,

    ZP_DISPLAY_FORMAT_R32_UINT,
    ZP_DISPLAY_FORMAT_R32_SINT,
    ZP_DISPLAY_FORMAT_R32_FLOAT,

    // RG Components
    ZP_DISPLAY_FORMAT_RG8_UINT,
    ZP_DISPLAY_FORMAT_RG8_SINT,

    ZP_DISPLAY_FORMAT_RG16_UINT,
    ZP_DISPLAY_FORMAT_RG16_SINT,
    ZP_DISPLAY_FORMAT_RG16_FLOAT,

    ZP_DISPLAY_FORMAT_RG32_UINT,
    ZP_DISPLAY_FORMAT_RG32_SINT,
    ZP_DISPLAY_FORMAT_RG32_FLOAT,

    // RGB Components
    ZP_DISPLAY_FORMAT_RGB8_UINT,
    ZP_DISPLAY_FORMAT_RGB8_SINT,

    ZP_DISPLAY_FORMAT_RGB16_UINT,
    ZP_DISPLAY_FORMAT_RGB16_SINT,
    ZP_DISPLAY_FORMAT_RGB16_FLOAT,

    ZP_DISPLAY_FORMAT_RGB32_UINT,
    ZP_DISPLAY_FORMAT_RGB32_SINT,
    ZP_DISPLAY_FORMAT_RGB32_FLOAT,

    // RGBA Components
    ZP_DISPLAY_FORMAT_RGBA8_UINT,
    ZP_DISPLAY_FORMAT_RGBA8_SINT,
    ZP_DISPLAY_FORMAT_RGBA8_UNORM,
    ZP_DISPLAY_FORMAT_RGBA8_SNORM,

    ZP_DISPLAY_FORMAT_RGBA16_UINT,
    ZP_DISPLAY_FORMAT_RGBA16_SINT,
    ZP_DISPLAY_FORMAT_RGBA16_UNORM,
    ZP_DISPLAY_FORMAT_RGBA16_SNORM,
    ZP_DISPLAY_FORMAT_RGBA16_FLOAT,

    ZP_DISPLAY_FORMAT_RGBA32_UINT,
    ZP_DISPLAY_FORMAT_RGBA32_SINT,
    ZP_DISPLAY_FORMAT_RGBA32_FLOAT,

    // Depth Buffer
    ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT,
    ZP_DISPLAY_FORMAT_D32_FLOAT,

    // Compressed
    ZP_DISPLAY_FORMAT_RGB_BC1,
    ZP_DISPLAY_FORMAT_RGBA_BC1,
    ZP_DISPLAY_FORMAT_RGBA_BC2,
    ZP_DISPLAY_FORMAT_RGBA_BC3,
    ZP_DISPLAY_FORMAT_ATI1N,
    ZP_DISPLAY_FORMAT_ATI2N,

    zpDisplayFormat_Count,
    zpDisplayFormat_Force32 = ZP_FORECE_32BIT,
};

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
    ZP_RENDERING_COMMNAD_NOOP = 0,

    ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT,
    ZP_RENDERING_COMMNAD_SET_VIEWPORT,

    ZP_RENDERING_COMMNAD_CLEAR,

    ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE,
    ZP_RENDERING_COMMNAD_DRAW_BUFFERED,
    ZP_RENDERING_COMMNAD_DRAW_INSTANCED,

    zpRenderingCommandType_Count,
    zpRenderingCommandType_Force32 = ZP_FORECE_32BIT
};

enum zpVertexFormat : zp_uint
{
    ZP_VERTEX_FORMAT_NONE = 0,

    ZP_VERTEX_FORMAT_VERTEX_COLOR,
    ZP_VERTEX_FORMAT_VERTEX_COLOR_UV,
    ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL,
    ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT,
    ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT_UV2,

    zpVertexFormat_Count,
    zpVertexFormat_Force32 = ZP_FORECE_32BIT
};

enum zpBufferType : zp_uint
{
    ZP_BUFFER_TYPE_UNKNOWN = 0,
    ZP_BUFFER_TYPE_VERTEX,
    ZP_BUFFER_TYPE_INDEX,
    ZP_BUFFER_TYPE_CONSTANT,
    ZP_BUFFER_TYPE_SHADER_RESOURCE,
    ZP_BUFFER_TYPE_STREAM_OUT,
    ZP_BUFFER_TYPE_RENDER_TARGET,
    ZP_BUFFER_TYPE_DEPTH_STENCIL,
    ZP_BUFFER_TYPE_UNORDERED_ACCESS,

    zpBufferType_Count,
    zpBufferType_Force32 = ZP_FORECE_32BIT
};

enum zpBufferBindType : zp_uint
{
    ZP_BUFFER_BIND_DEFAULT = 0,
    ZP_BUFFER_BIND_DYNAMIC,
    ZP_BUFFER_BIND_STREAM,
    ZP_BUFFER_BIND_IMMUTABLE,

    zpBufferBindType_Count,
    zpBufferBindType_Force32 = ZP_FORECE_32BIT
};

enum zpMapType : zp_uint
{
    ZP_MAP_TYPE_READ = 0,
    ZP_MAP_TYPE_WRITE,
    ZP_MAP_TYPE_READ_WRITE,
    ZP_MAP_TYPE_WRITE_DISCARD,
    ZP_MAP_TYPE_WRITE_NO_OVERWRITE,

    zpMapType_Count,
    zpMapType_Force32 = ZP_FORECE_32BIT
};

enum zpTextureDimension : zp_uint
{
    ZP_TEXTURE_DIMENSION_UNKNOWN = 0,
    ZP_TEXTURE_DIMENSION_1D,
    ZP_TEXTURE_DIMENSION_2D,
    ZP_TEXTURE_DIMENSION_3D,
    ZP_TEXTURE_DIMENSION_CUBE_MAP,

    zpTextureDimension_Count,
    zpTextureDimension_Force32 = ZP_FORECE_32BIT
};

enum zpTextureType : zp_uint
{
    ZP_TEXTURE_TYPE_TEXTURE = 0,
    ZP_TEXTURE_TYPE_RENDER_TARGET,
    ZP_TEXTURE_TYPE_RENDER_TEXTURE,

    zpTextureType_Count,
    zpTextureType_Force32 = ZP_FORECE_32BIT
};

enum zpViewportType : zp_uint
{
    ZP_VIEWPORT_TYPE_FULLSCREEN = 0,

    ZP_VIEWPORT_TYPE_TOP,
    ZP_VIEWPORT_TYPE_BOTTOM,

    ZP_VIEWPORT_TYPE_TOP_LEFT,
    ZP_VIEWPORT_TYPE_TOP_RIGHT,
    ZP_VIEWPORT_TYPE_BOTTOM_LEFT,
    ZP_VIEWPORT_TYPE_BOTTOM_RIGHT,

    zpViewportType_Count,
    zpViewportType_Force32 = ZP_FORECE_32BIT
};

union zpRenderHandle
{
    zp_handle ptr;
    zp_uint index;
};

struct zpRenderBuffer
{
    zp_size_t size;
    zp_size_t stride;
    zpBufferType bufferType;
    zpBufferBindType bindType;

    zpRenderHandle buffer;
};

struct zpViewport
{
    zp_int topX;
    zp_int topY;
    zp_int width;
    zp_int height;
    zp_float minDepth;
    zp_float maxDepth;
};

union zpRenderingSortKey
{
    struct
    {
        zp_bool anchor : 1;
        zp_byte viewport : 3;
        zp_byte layer : 4;
        zp_uint depth : 24;
        zp_uint material : 30;
        zp_byte pass : 2;
    };
    zp_ulong key;
};

class zpRenderingEngine;

#include "zpTexture.h"
#include "zpShader.h"
#include "zpMaterial.h"
#include "zpFont.h"
#include "zpMesh.h"
#include "zpCamera.h"

struct zpRenderingCommand
{
    zpRenderingCommandType type;
    zp_uint id;
    zpRenderingSortKey sortKey;

    // draw command data
    zpMatrix4fData transform;

    zpVertexFormat vertexFormat;
    zpTopology topology;

    zp_size_t vertexOffset;
    zp_size_t vertexCount;
    zp_size_t indexOffset;
    zp_size_t indexCount;

    zpRenderBuffer vertexBuffer;
    zpRenderBuffer indexBuffer;

    zpMaterialHandle material;
    zpFontHandle font;
    zpMeshHandle mesh;

    // draw instanced data
    zp_hash64 instanceId;

    // scissor rect data
    zpRecti scissorRect;

    // viewport rect data
    zpViewport viewport;

    // clear data
    zpColorf clearColor;
    zp_float clearDepth;
    zp_int clearStencil;
};

#include "zpRenderingContext.h"
#include "zpRenderingEngine.h"

#endif // !ZP_RENDERING_H
