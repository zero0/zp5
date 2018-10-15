#pragma once
#ifndef ZP_RENDERING_H
#define ZP_RENDERING_H

#include "Core\zpCore.h"

enum zpRenderingConstants
{
    ZP_MAX_GLOBAL_NAME_SIZE = 32,
    ZP_MAX_MARKER_NAME_SIZE = 32,

};

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
    zpDisplayFormat_Force32 = ZP_FORCE_32BIT,
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
    zpTopology_Force32 = ZP_FORCE_32BIT
};

enum zpRenderCommandType : zp_uint
{
    ZP_RENDER_COMMNAD_NOOP = 0,

    ZP_RENDER_COMMNAD_PUSH_MARKER,
    ZP_RENDER_COMMNAD_POP_MARKER,

    ZP_RENDER_COMMNAD_SET_VIEWPORT,

    ZP_RENDER_COMMNAD_SET_SCISSOR_RECT,

    ZP_RENDER_COMMNAD_CLEAR_COLOR_DEPTH_STENCIL,
    ZP_RENDER_COMMNAD_CLEAR_COLOR,
    ZP_RENDER_COMMNAD_CLEAR_COLOR_DEPTH,
    ZP_RENDER_COMMNAD_CLEAR_DEPTH_STENCIL,
    ZP_RENDER_COMMNAD_CLEAR_DEPTH,
    ZP_RENDER_COMMNAD_CLEAR_STENCIL,

    ZP_RENDER_COMMNAD_SET_RT_COLOR,
    ZP_RENDER_COMMNAD_SET_RT_COLOR_DEPTH,
    ZP_RENDER_COMMNAD_SET_RT_COLORS,
    ZP_RENDER_COMMNAD_SET_RT_COLORS_DEPTH, 
    ZP_RENDER_COMMNAD_SET_RT_DEPTH,

    ZP_RENDER_COMMNAD_BLIT_RT,
    ZP_RENDER_COMMNAD_BLIT_TEXTURE,

    ZP_RENDER_COMMNAD_SET_GLOBAL_INT,
    ZP_RENDER_COMMNAD_SET_GLOBAL_FLOAT,
    ZP_RENDER_COMMNAD_SET_GLOBAL_COLOR,
    ZP_RENDER_COMMNAD_SET_GLOBAL_VECTOR,
    ZP_RENDER_COMMNAD_SET_GLOBAL_MATRIX,
    ZP_RENDER_COMMNAD_SET_GLOBAL_TEXTURE,
    ZP_RENDER_COMMNAD_SET_GLOBAL_BUFFER,

    zpRenderCommandType_Count,

    ZP_RENDER_COMMNAD_PRESENT,

    ZP_RENDER_COMMNAD_DRAW_MESH,
    ZP_RENDER_COMMNAD_DRAW_MESH_INSTANCED,
    ZP_RENDER_COMMNAD_DRAW_MESH_INSTANCED_INDIRECT,

    zpRenderCommandType_Force32 = ZP_FORCE_32BIT
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
    zpVertexFormat_Force32 = ZP_FORCE_32BIT
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
    zpBufferType_Force32 = ZP_FORCE_32BIT
};

enum zpBufferBindType : zp_uint
{
    ZP_BUFFER_BIND_DEFAULT = 0,
    ZP_BUFFER_BIND_DYNAMIC,
    ZP_BUFFER_BIND_STREAM,
    ZP_BUFFER_BIND_IMMUTABLE,

    zpBufferBindType_Count,
    zpBufferBindType_Force32 = ZP_FORCE_32BIT
};

enum zpMapType : zp_uint
{
    ZP_MAP_TYPE_READ = 0,
    ZP_MAP_TYPE_WRITE,
    ZP_MAP_TYPE_READ_WRITE,
    ZP_MAP_TYPE_WRITE_DISCARD,
    ZP_MAP_TYPE_WRITE_NO_OVERWRITE,

    zpMapType_Count,
    zpMapType_Force32 = ZP_FORCE_32BIT
};

enum zpTextureDimension : zp_uint
{
    ZP_TEXTURE_DIMENSION_UNKNOWN = 0,
    ZP_TEXTURE_DIMENSION_1D,
    ZP_TEXTURE_DIMENSION_2D,
    ZP_TEXTURE_DIMENSION_3D,
    ZP_TEXTURE_DIMENSION_CUBE_MAP,

    zpTextureDimension_Count,
    zpTextureDimension_Force32 = ZP_FORCE_32BIT
};

enum zpTextureType : zp_uint
{
    ZP_TEXTURE_TYPE_TEXTURE = 0,
    ZP_TEXTURE_TYPE_RENDER_TARGET,
    ZP_TEXTURE_TYPE_RENDER_TEXTURE,

    zpTextureType_Count,
    zpTextureType_Force32 = ZP_FORCE_32BIT
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
    zpViewportType_Force32 = ZP_FORCE_32BIT
};

enum zpRenderSortOrder : zp_uint
{
    ZP_RENDER_SORT_ORDER_NONE = 0,

    ZP_RENDER_SORT_ORDER_FRONT_TO_BACK,
    ZP_RENDER_SORT_ORDER_BACK_TO_FRONT,

    zpRenderSortOrder_Count,
    zpRenderSortOrder_Force32 = ZP_FORCE_32BIT
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

union zpRenderSortKey
{
    struct
    {
        zp_byte viewport : 3;
        zp_byte layer : 4;
        zp_int depth : 24;
        zp_uint material : 30;
        zp_byte pass : 3;
    };
    zp_ulong key;
};

struct zpSortRenderCommandData
{
    zpRenderSortKey key;
    zp_size_t index;
};

struct zpRenderRange
{
    zp_uint minRenderQueue;
    zp_uint maxRenderQueue;
};

struct zpDrawRenderersDesc
{
    zpRenderSortOrder sortOrder;
    zpRenderRange renderRange;
    zp_flag32 renderLayers;
    zp_uint passIndex;
    zp_flag32 viewPorts;
};

struct zpRenderingStat
{
    zp_ulong frameIndex;
    zp_ulong frameTime;
    zp_ulong primitiveCount;
    zp_ulong samplesPassed;
};

class zpRenderingEngine;

#include "zpTexture.h"
#include "zpRenderTarget.h"
#include "zpShader.h"
#include "zpMaterial.h"
#include "zpFont.h"
#include "zpMesh.h"
#include "zpCamera.h"

struct zpDrawRendererCommand
{
    zpRenderSortKey sortKey;

    zpMatrix4fData localToWorld;
    zpMatrix4fData worldToLocal;

    zpBoundingAABB bounds;

    zpVertexFormat vertexFormat;
    zpTopology topology;

    zp_size_t vertexOffset;
    zp_size_t vertexCount;
    zp_size_t indexOffset;
    zp_size_t indexCount;

    zpRenderBuffer vertexBuffer;
    zpRenderBuffer indexBuffer;

    zpMaterialHandle material;
    zpMeshHandle mesh;
};

struct zpRenderingCommand
{
    zpRenderCommandType type;
    zp_uint id;
    zpRenderSortKey sortKey;

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

struct zpRenderCommandHeader
{
    zpRenderCommandType type;
    zp_uint id;
};

struct zpRenderCommandPushMarker
{
    zpRenderCommandHeader header;
    zp_char marker[ ZP_MAX_MARKER_NAME_SIZE ];
    zp_size_t length;
};

struct zpRenderCommandPopMarker
{
    zpRenderCommandHeader header;
};

struct zpRenderCommandSetViewport
{
    zpRenderCommandHeader header;
    zpViewport viewport;
};

struct zpRenderCommandSetScissorRect
{
    zpRenderCommandHeader header;
    zpRecti scissorRect;
};

struct zpRenderCommandClear
{
    zpRenderCommandHeader header;
    zpColorf color;
    zp_float depth;
    zp_int stencil;
};

struct zpRenderCommandClearColor
{
    zpRenderCommandHeader header;
    zpColorf color;
};

struct zpRenderCommandClearColorDepth
{
    zpRenderCommandHeader header;
    zpColorf color;
    zp_float depth;
};

struct zpRenderCommandClearDepthStencil
{
    zpRenderCommandHeader header;
    zp_float depth;
    zp_int stencil;
};

struct zpRenderCommandClearDepth
{
    zpRenderCommandHeader header;
    zp_float depth;
};

struct zpRenderCommandClearStencil
{
    zpRenderCommandHeader header;
    zp_int stencil;
};

struct zpRenderCommandSetRenderTargetColor
{
    zpRenderCommandHeader header;
    zpRenderTargetIdentifier rtColor;
};

struct zpRenderCommandSetRenderTargetColorDepth
{
    zpRenderCommandHeader header;
    zpRenderTargetIdentifier rtColor;
    zpRenderTargetIdentifier rtDepth;
};

struct zpRenderCommandSetRenderTargetColors
{
    zpRenderCommandHeader header;
    zpRenderTargetIdentifier rtColor[ 8 ];
    zp_size_t rtCount;
};

struct zpRenderCommandSetRenderTargetColorsDepth
{
    zpRenderCommandHeader header;
    zpRenderTargetIdentifier rtColors[8];
    zpRenderTargetIdentifier rtDepth;
    zp_size_t rtCount;
};

struct zpRenderCommandSetRenderTargetDepth
{
    zpRenderCommandHeader header;
    zpRenderTargetIdentifier rtDepth;
};

struct zpRenderCommandDrawRenderers
{
    zpRenderCommandHeader header;
    zpDrawRenderersDesc desc;
};

struct zpRenderCommandBlitRenderTexture
{
    zpRenderCommandHeader header;
};

struct zpRenderCommandBlitTexture
{
    zpRenderCommandHeader header;
};

struct zpRenderCommandSetGlobalInt
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zp_int value;
};

struct zpRenderCommandSetGlobalFloat
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zp_float value;
};

struct zpRenderCommandSetGlobalColor
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zpColorf value;
};

struct zpRenderCommandSetGlobalVector
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zpVector4fData value;
};

struct zpRenderCommandSetGlobalMatrix
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zpMatrix4fData value;
};

struct zpRenderCommandSetGlobalTexture
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zpRenderHandle value;
};

struct zpRenderCommandSetGlobalBuffer
{
    zpRenderCommandHeader header;
    zp_char globalName[ ZP_MAX_GLOBAL_NAME_SIZE ];
    zpRenderHandle value;
    zp_size_t offset;
    zp_size_t length;
    zp_size_t stride;
};

//
// Internal Commands
//

struct zpRenderCommandPresent
{
    zpRenderCommandHeader header;
    zp_handle hDC;
    zp_handle hContext;
    zp_ulong frameIndex;
};

struct zpRenderCommandDrawMesh
{
    zpRenderCommandHeader header;
    zpRenderBuffer vertexBuffer;
    zpRenderBuffer indexBuffer;
    zp_size_t vertexOffset;
    zp_size_t vertexCount;
    zp_size_t indexOffset;
    zp_size_t indexCount;
    zp_uint indexStride;
    zpVertexFormat vertexFormat;
    zpTopology topology;

    const zpMaterial* material;
    zp_int passIndex;

    zpMatrix4fData localToWorld;
};

//
//
//

struct zpDrawRenderable
{
    zp_flag32 renderLayers;
    zp_int passIndex;
    zp_int subMeshIndex;

    zpMeshHandle mesh;
    zpMaterialHandle material;

    zpMatrix4fData localToWorld;
    zpBoundingAABB bounds;
};

struct zpDrawRenderableSort
{
    zpRenderSortKey sortKey;
    const zpDrawRenderable* drawRenderable;
};


#include "zpRenderCommandBuffer.h"
#include "zpRenderContext.h"

#include "zpRenderPipeline.h"
#include "zpRenderingContext.h"
#include "zpRenderingEngine.h"

#endif // !ZP_RENDERING_H
