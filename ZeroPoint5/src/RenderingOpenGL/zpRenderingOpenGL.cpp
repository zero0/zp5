#include "Core\zpCore.h"
#include "RenderingOpenGL\zpRenderingOpenGL.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

#include "GL\glew.h"
#include "GL\wglew.h"

#pragma comment( lib, "opengl32.lib" )

static ZP_FORCE_INLINE GLenum _TopologyToMode( zpTopology topology )
{
    constexpr GLenum mapping[] =
    {
        0,
        GL_POINTS,

        GL_LINES,
        GL_LINE_STRIP,

        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
    };

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpTopology_Count );
    return mapping[ topology ];
}

static ZP_FORCE_INLINE GLenum _BufferTypeToTarget( zpBufferType type )
{
    constexpr GLenum mapping[] =
    {
        0,
        GL_ARRAY_BUFFER,
        GL_ELEMENT_ARRAY_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
        0,
        0,
        0,
        0,
    };

    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpBufferType_Count );
    return mapping[ type ];
}

static ZP_FORCE_INLINE GLenum _BufferBindTypeToUsage( zpBufferBindType bindType )
{
    constexpr GLenum mapping[] =
    {
        0,
        GL_DYNAMIC_DRAW,
        GL_STREAM_DRAW,
        GL_STATIC_DRAW
    };

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpBufferBindType_Count );
    return mapping[ bindType ];
}

static ZP_FORCE_INLINE GLenum _TextureDimensionToTarget( zpTextureDimension textureDimenision )
{
    constexpr GLenum mapping[] =
    {
        0,
        GL_TEXTURE_1D,
        GL_TEXTURE_1D_ARRAY,
        GL_TEXTURE_2D,
        GL_TEXTURE_2D_ARRAY,
        GL_TEXTURE_2D_MULTISAMPLE,
        GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
        GL_TEXTURE_3D,
        GL_TEXTURE_CUBE_MAP,
    };

    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpTextureDimension_Count );
    return mapping[ textureDimenision ];
}

static ZP_FORCE_INLINE GLint _DisplayFormatToInternalFormat( zpDisplayFormat displayFormat )
{
    constexpr GLint mapping[] =
    {
        0,

        // R Component
        GL_R8,
        GL_R8_SNORM,

        GL_R16,
        GL_R16_SNORM,
        GL_R16F,

        GL_R32UI,
        GL_R32I,
        GL_R32F,

        // RG Components
        GL_RG8,
        GL_RG8_SNORM,

        GL_RG16,
        GL_RG16_SNORM,
        GL_RG16F,

        GL_RG32UI,
        GL_RG32I,
        GL_RG32F,

        // RGB Components
        GL_RGB8,
        GL_RGB8_SNORM,
        GL_SRGB8,

        GL_RGB16,
        GL_RGB16_SNORM,
        GL_RGB16F,

        GL_RGB32UI,
        GL_RGB32I,
        GL_RGB32F,

        // RGBA Components
        GL_RGBA8UI,
        GL_RGBA8I,
        GL_RGBA,
        GL_RGBA_SNORM,
        GL_SRGB8_ALPHA8,

        GL_RGBA16UI,
        GL_RGBA16I,
        GL_RGBA16,
        GL_RGBA16_SNORM,
        GL_RGBA16F,

        GL_RGBA32UI,
        GL_RGBA32I,
        GL_RGBA32F,

        // Depth Buffer
        GL_DEPTH24_STENCIL8,
        GL_DEPTH_COMPONENT32F,

        // Compressed
        GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
        GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
        GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
        GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
        0,
        0,
    };

    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpDisplayFormat_Count );
    return mapping[ displayFormat ];
}

static ZP_FORCE_INLINE GLenum _DisplayFormatToFormat( zpDisplayFormat displayFormat )
{
    constexpr GLint mapping[] =
    {
        0,

        // R Component
        GL_RED,
        GL_RED,

        GL_RED,
        GL_RED,
        GL_RED,

        GL_RED_INTEGER,
        GL_RED_INTEGER,
        GL_RED,

        // RG Components
        GL_RG,
        GL_RG,

        GL_RG,
        GL_RG,
        GL_RG,

        GL_RG_INTEGER,
        GL_RG_INTEGER,
        GL_RG,

        // RGB Components
        GL_RGB,
        GL_RGB,
        GL_RGB,

        GL_RGB,
        GL_RGB,
        GL_RGB,

        GL_RGB_INTEGER,
        GL_RGB_INTEGER,
        GL_RGB,

        // RGBA Components
        GL_RGBA_INTEGER,
        GL_RGBA_INTEGER,
        GL_RGBA,
        GL_RGBA,
        GL_RGBA,

        GL_RGBA_INTEGER,
        GL_RGBA_INTEGER,
        GL_RGBA,
        GL_RGBA,
        GL_RGBA,

        GL_RGBA_INTEGER,
        GL_RGBA_INTEGER,
        GL_RGBA,

        // Depth Buffer
        GL_DEPTH_STENCIL,
        GL_DEPTH_COMPONENT,

        // Compressed
        GL_COMPRESSED_RGB,
        GL_COMPRESSED_RGBA,
        GL_COMPRESSED_RGBA,
        GL_COMPRESSED_RGBA,
        0,
        0,
    };

    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpDisplayFormat_Count );
    return mapping[ displayFormat ];
}

static ZP_FORCE_INLINE GLenum _DisplayFormatToDataType( zpDisplayFormat displayFormat )
{
    constexpr GLenum mapping[] =
    {
        0,

        // R Component
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE,

        GL_UNSIGNED_SHORT,
        GL_SHORT,
        GL_FLOAT,

        GL_UNSIGNED_INT,
        GL_INT,
        GL_FLOAT,

        // RG Components
        GL_UNSIGNED_BYTE,
        GL_BYTE,

        GL_UNSIGNED_SHORT,
        GL_SHORT,
        GL_FLOAT,

        GL_UNSIGNED_INT,
        GL_INT,
        GL_FLOAT,

        // RGB Components
        GL_UNSIGNED_BYTE,
        GL_BYTE,
        GL_UNSIGNED_BYTE,

        GL_UNSIGNED_SHORT,
        GL_SHORT,
        GL_FLOAT,

        GL_UNSIGNED_INT,
        GL_INT,
        GL_FLOAT,

        // RGBA Components
        GL_UNSIGNED_BYTE,
        GL_BYTE,
        GL_UNSIGNED_BYTE,
        GL_BYTE,
        GL_UNSIGNED_BYTE,

        GL_UNSIGNED_SHORT,
        GL_SHORT,
        GL_UNSIGNED_SHORT,
        GL_SHORT,
        GL_FLOAT,

        GL_UNSIGNED_INT,
        GL_INT,
        GL_FLOAT,

        // Depth Buffer
        GL_FLOAT,
        GL_FLOAT,

        // Compressed
        0,
        0,
        0,
        0,
        0,
        0,
    };

    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpDisplayFormat_Count );
    return mapping[ displayFormat ];
}

static ZP_FORCE_INLINE zp_bool _IsCompressedDisplayFormat( zpDisplayFormat displayFormat )
{
    switch( displayFormat )
    {
        case ZP_DISPLAY_FORMAT_RGB_BC1:
        case ZP_DISPLAY_FORMAT_RGBA_BC1:
        case ZP_DISPLAY_FORMAT_RGBA_BC2:
        case ZP_DISPLAY_FORMAT_RGBA_BC3:
        case ZP_DISPLAY_FORMAT_ATI1N:
        case ZP_DISPLAY_FORMAT_ATI2N:
            return true;
    }

    return false;
}

static ZP_FORCE_INLINE GLsizei _GetCompressedImageSize( zp_uint width, zp_uint height, zp_uint depth, zp_uint blockSize )
{
    GLsizei imageSize = ( ( width + 3 ) / 4 ) * ( ( height + 3 ) / 4 ) * ( ( depth + 3 ) / 4 ) * blockSize;
    return imageSize;
}

static ZP_FORCE_INLINE zp_bool _IsAntiAliased( zpTextureDimension dimension )
{
    switch( dimension )
    {
        case ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE:
        case ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE_ARRAY:
            return true;
    }

    return false;
}

static ZP_FORCE_INLINE zp_bool _IsDepthFormat( zpDisplayFormat format )
{
    switch( format )
    {
        case ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT:
        case ZP_DISPLAY_FORMAT_D32_FLOAT:
            return true;
    }

    return false;
}

static ZP_FORCE_INLINE GLenum _DisplayFormatToAttachment( zpDisplayFormat format, zp_uint colorIndex = 0 )
{
    GLenum attachement = GL_COLOR_ATTACHMENT0 + colorIndex;

    switch( format )
    {
        case ZP_DISPLAY_FORMAT_D32_FLOAT:
            attachement = GL_DEPTH_ATTACHMENT;
            break;

        case ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT:
            attachement = GL_DEPTH_STENCIL_ATTACHMENT;
            break;
    }

    return attachement;
}

static ZP_FORCE_INLINE GLenum _BlendModeToMode( zpBlendMode mode )
{
    constexpr GLenum mapping[] =
    {
        GL_ZERO,
        GL_ONE,
        GL_DST_COLOR,
        GL_SRC_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_ALPHA,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpBlendMode_Count );
    return mapping[ mode ];
}

static ZP_FORCE_INLINE GLenum _BlendOperationToMode( zpBlendOperation operation )
{
    constexpr GLenum mapping[] =
    {
        GL_FUNC_ADD,
        GL_FUNC_SUBTRACT,
        GL_FUNC_REVERSE_SUBTRACT,
        GL_MIN,
        GL_MAX,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpBlendOperation_Count );
    return mapping[ operation ];
}

static ZP_FORCE_INLINE GLenum _CullModeToMode( zpCullMode mode )
{
    constexpr GLenum mapping[] =
    {
        0,
        GL_FRONT,
        GL_BACK,
        GL_FRONT_AND_BACK,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpCullMode_Count );
    return mapping[ mode ];
}

static ZP_FORCE_INLINE GLenum _FrontFaceToMode( zpFrontFace mode )
{
    constexpr GLenum mapping[] =
    {
        GL_CW,
        GL_CCW,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpFrontFace_Count );
    return mapping[ mode ];
}

static ZP_FORCE_INLINE GLenum _StencilOperationToMode( zpStencilOperation operation )
{
    constexpr GLenum mapping[] =
    {
        GL_KEEP,
        GL_ZERO,
        GL_REPLACE,
        GL_INVERT,
        GL_INCR,
        GL_DECR,
        GL_INCR_WRAP,
        GL_DECR_WRAP,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpStencilOperation_Count );
    return mapping[ operation ];
}

static ZP_FORCE_INLINE GLenum _CompareFunctionToMode( zpCompareFunction func )
{
    constexpr GLenum mapping[] =
    {
        0,
        GL_NEVER,
        GL_LESS,
        GL_LEQUAL,
        GL_GREATER,
        GL_GEQUAL,
        GL_EQUAL,
        GL_NOTEQUAL,
        GL_ALWAYS,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpCompareFunction_Count );
    return mapping[ func ];
}

static ZP_FORCE_INLINE GLenum _FillModeToMode( zpFillMode mode )
{
    constexpr GLenum mapping[] =
    {
        GL_POINT,
        GL_LINE,
        GL_FILL,
    };
    ZP_STATIC_ASSERT( ZP_ARRAY_SIZE( mapping ) == zpFillMode_Count );
    return mapping[ mode ];
}

union glQuery
{
    struct
    {
        GLuint timeElapsed;
        GLuint primitivesGenerated;
        GLuint samplesPassed;
    };
    GLuint q[ 3 ];
};

static ZP_CONSTEXPR zp_uint NUM_FBO = 2;

static GLuint g_fbo[ NUM_FBO ];
static GLuint g_vaos[ zpVertexFormat_Count ];
static zpShader g_shaderVC;
static zpShader g_shaderVCU;
#ifdef ZP_USE_PROFILER
static ZP_CONSTEXPR zp_uint NUM_QUERIES = 2;
static glQuery g_queries[ NUM_QUERIES ];
#endif

static void BindVertexFormatForRenderCommand( const zpRenderCommandDrawMesh* cmd )
{
    constexpr zp_int strides[] =
    {
        sizeof( zp_float ) * 0,
        sizeof( zp_float ) * ( 4 + 1 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 + 4 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 + 4 + 4 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 + 4 + 4 + 2 )
    };
    constexpr zp_size_t offsets[] =
    {
        sizeof( zp_float ) * 0,
        sizeof( zp_float ) * ( 4 ),
        sizeof( zp_float ) * ( 4 + 1 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 + 4 ),
        sizeof( zp_float ) * ( 4 + 1 + 2 + 4 + 4 )
    };
    ZP_STATIC_ASSERT( ( sizeof( strides ) / sizeof( strides[ 0 ] ) ) == zpVertexFormat_Count );
    ZP_STATIC_ASSERT( ( sizeof( offsets ) / sizeof( offsets[ 0 ] ) ) == zpVertexFormat_Count );

    GLuint prog = 0;
    if( cmd->material && cmd->material->shader.isValid() )
    {
        prog = cmd->material->shader->programShader.index;
    }

    // TODO: remove when testing complete
    switch( cmd->vertexFormat )
    {
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
            prog = ( g_shaderVC.programShader.index );
            break;

        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
            prog = ( g_shaderVCU.programShader.index );
            break;
    }

    glUseProgram( prog );

    zp_int stride = strides[ cmd->vertexFormat ];

    GLuint vao = g_vaos[ cmd->vertexFormat ];
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, cmd->vertexBuffer.buffer.index );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, cmd->indexBuffer.buffer.index );

    zp_size_t vertexOffset = cmd->vertexOffset;

    switch( cmd->vertexFormat )
    {
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT_UV2:
        {
            GLint uv1Location = glGetAttribLocation( prog, "uv1" );
            glVertexAttribPointer( uv1Location, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 5 ] ) );
            glEnableVertexAttribArray( uv1Location );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT:
        {
            GLint tangentLocation = glGetAttribLocation( prog, "tangent" );
            glVertexAttribPointer( tangentLocation, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 4 ] ) );
            glEnableVertexAttribArray( tangentLocation );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL:
        {
            GLint normalLocation = glGetAttribLocation( prog, "normal" );
            glVertexAttribPointer( normalLocation, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 3 ] ) );
            glEnableVertexAttribArray( normalLocation );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
        {
            GLint uv0Location = glGetAttribLocation( prog, "uv0" );
            glVertexAttribPointer( uv0Location, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 2 ] ) );
            glEnableVertexAttribArray( uv0Location );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
        {
            GLint colorLocation = glGetAttribLocation( prog, "color" );
            glVertexAttribPointer( colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 1 ] ) );
            glEnableVertexAttribArray( colorLocation );
            GLint vertexLocation = glGetAttribLocation( prog, "vertex" );
            glVertexAttribPointer( vertexLocation, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 0 ] ) );
            glEnableVertexAttribArray( vertexLocation );
        } break;

        default:
            break;
    }

    GLint otw = glGetUniformLocation( prog, "_ObjectToWorld" );
    glUniformMatrix4fv( otw, 1, GL_FALSE, cmd->localToWorld.m );

    if( cmd->material )
    {
        // set ints
        {
            zpMap< zpShaderProperty, zp_int >::const_iterator b = cmd->material->ints.begin();
            zpMap< zpShaderProperty, zp_int >::const_iterator e = cmd->material->ints.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glUniform1i( c, b.value() );
                }
            }
        }

        // set floats
        {
            zpMap< zpShaderProperty, zp_float >::const_iterator b = cmd->material->floats.begin();
            zpMap< zpShaderProperty, zp_float >::const_iterator e = cmd->material->floats.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glUniform1f( c, b.value() );
                }
            }
        }

        // set colors
        {
            zpMap< zpShaderProperty, zpColorf >::const_iterator b = cmd->material->colors.begin();
            zpMap< zpShaderProperty, zpColorf >::const_iterator e = cmd->material->colors.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glUniform4fv( c, 1, b.value().rgba );
                }
            }
        }

        // set vectors
        {
            zpMap< zpShaderProperty, zpVector4f >::const_iterator b = cmd->material->vectors.begin();
            zpMap< zpShaderProperty, zpVector4f >::const_iterator e = cmd->material->vectors.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glUniform4fv( c, 1, b.value().m );
                }
            }
        }

        // set matrices
        {
            zpMap< zpShaderProperty, zpMatrix4f >::const_iterator b = cmd->material->matrices.begin();
            zpMap< zpShaderProperty, zpMatrix4f >::const_iterator e = cmd->material->matrices.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glUniformMatrix4fv( c, 1, GL_FALSE, b.value().m );
                }
            }
        }

        // set textures
        {
            zp_int i = 0;
            zpMap< zpShaderProperty, zpTextureHandle >::const_iterator b = cmd->material->textures.begin();
            zpMap< zpShaderProperty, zpTextureHandle >::const_iterator e = cmd->material->textures.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glActiveTexture( GL_TEXTURE0 + i );
                    glBindTexture( GL_TEXTURE_2D, b.value()->texture.index );
                    glUniform1i( c, i );

                    ++i;
                }
            }
        }
    }

    // TODO: update material to set these
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glDepthFunc( GL_LEQUAL );
    //glDepthMask( GL_TRUE );
}

static void UnbindVertexFormatForRenderCommand( const zpRenderCommandDrawMesh* cmd )
{
    GLuint prog = 0;
    if( cmd->material && cmd->material->shader.isValid() )
    {
        prog = cmd->material->shader->programShader.index;
    }

    // TODO: remove when testing complete
    switch( cmd->vertexFormat )
    {
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
            prog = ( g_shaderVC.programShader.index );
            break;

        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
            prog = ( g_shaderVCU.programShader.index );
            break;
    }

    switch( cmd->vertexFormat )
    {
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT_UV2:
        {
            GLint uv1Location = glGetAttribLocation( prog, "uv1" );
            glDisableVertexAttribArray( uv1Location );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT:
        {
            GLint tangentLocation = glGetAttribLocation( prog, "tangent" );
            glDisableVertexAttribArray( tangentLocation );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL:
        {
            GLint normalLocation = glGetAttribLocation( prog, "normal" );
            glDisableVertexAttribArray( normalLocation );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
        {
            GLint uv0Location = glGetAttribLocation( prog, "uv0" );
            glDisableVertexAttribArray( uv0Location );
        }
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
        {
            GLint colorLocation = glGetAttribLocation( prog, "color" );
            glDisableVertexAttribArray( colorLocation );
            GLint vertexLocation = glGetAttribLocation( prog, "vertex" );
            glDisableVertexAttribArray( vertexLocation );
        } break;

        default:
            break;
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    if( cmd->material )
    {
        // unset textures
        {
            zp_int i = 0;
            zpMap< zpShaderProperty, zpTextureHandle >::const_iterator b = cmd->material->textures.begin();
            zpMap< zpShaderProperty, zpTextureHandle >::const_iterator e = cmd->material->textures.end();
            for( ; b != e; ++b )
            {
                GLint c = glGetUniformLocation( prog, b.key().name() );
                if( c >= 0 )
                {
                    glActiveTexture( GL_TEXTURE0 + i );
                    glBindTexture( GL_TEXTURE_2D, 0 );

                    ++i;
                }
            }
        }
    }

    glUseProgram( 0 );
}


#if 0
struct glDebugBlockS
{
public:
    ZP_FORCE_INLINE glDebugBlockS( GLenum source, const char* message );
    ZP_FORCE_INLINE ~glDebugBlockS();
};

ZP_FORCE_INLINE glDebugBlockS::glDebugBlockS( GLenum source, const char* message )
{
    glPushDebugGroup( source, 1, -1, message );
}
ZP_FORCE_INLINE glDebugBlockS::~glDebugBlockS()
{
    glPopDebugGroup();
}

#define glDebugBlock( source, message ) glDebugBlockS( source, message )
#else
#define glDebugBlock( source, message ) (void)0
#endif

#if ZP_DEBUG
static ZP_FORCE_INLINE void glMarkerPushS( const zp_char* marker, GLsizei length )
{
    if( GLEW_EXT_debug_marker )
    {
        glPushGroupMarkerEXT( length, marker );
    }
    else
    {
        glPushDebugGroup( GL_DEBUG_SOURCE_APPLICATION, 0, length, marker );
    }
}
static ZP_FORCE_INLINE void glMarkerPopS()
{
    if( GLEW_EXT_debug_marker )
    {
        glPopGroupMarkerEXT();
    }
    else
    {
        glPopDebugGroup();
    }
}

struct glMarkerBlockS
{
    ZP_FORCE_INLINE glMarkerBlockS( const char* marker );
    ZP_FORCE_INLINE ~glMarkerBlockS();
};

ZP_FORCE_INLINE glMarkerBlockS::glMarkerBlockS( const char* marker )
{
    glMarkerPushS( marker, -1 );
}
ZP_FORCE_INLINE glMarkerBlockS::~glMarkerBlockS()
{
    glMarkerPopS();
}

#define glMarkerBlock( marker )         glMarkerBlockS ZP_CONCAT( __marker_block_, __LINE__)( marker )
#define glMarkerPush( marker, length )  glMarkerPushS( marker, length )
#define glMarkerPop()                   glMarkerPopS()
#else
#define glMarkerBlock( ... )            (void)0
#define glMarkerPush( marker )          (void)0
#define glMarkerPush()                  (void)0
#endif

static void GLAPIENTRY _DebugOutputOpenGL( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam )
{
    const zp_char* src = "   ";
    const zp_char* typ = "   ";
    const zp_char* sev = " ";

    switch( source )
    {
        case GL_DEBUG_SOURCE_API            : src = "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM  : src = "WND"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "CGC"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY    : src = "3RD"; break;
        case GL_DEBUG_SOURCE_APPLICATION    : src = "APP"; break;
        case GL_DEBUG_SOURCE_OTHER          : src = "OTH"; break;
    }

    switch( type )
    {
        case GL_DEBUG_TYPE_ERROR              : typ = "ERR"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typ = "DEP"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR : typ = "UND"; break;
        case GL_DEBUG_TYPE_PORTABILITY        : typ = "PRT"; break;
        case GL_DEBUG_TYPE_PERFORMANCE        : typ = "PER"; break;
        case GL_DEBUG_TYPE_OTHER              : typ = "OTH"; break;
        case GL_DEBUG_TYPE_MARKER             : typ = "MRK"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP         : typ = "PUG"; break;
        case GL_DEBUG_TYPE_POP_GROUP          : typ = "POG"; break;
    }

    switch( severity )
    {
        case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "i"; break;
        case GL_DEBUG_SEVERITY_HIGH        : sev = "!"; break;
        case GL_DEBUG_SEVERITY_MEDIUM      : sev = "+"; break;
        case GL_DEBUG_SEVERITY_LOW         : sev = "-"; break;
    }

    zp_printfln( "%s %s %s: %s", sev, src, typ, message );
}

#if ZP_DEBUG
#define glAssert( t, m ) do { if( !(t) ) { glDebugMessageInsert( GL_DEBUG_SOURCE_THIRD_PARTY, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_LOW, -1, #t ": " m ); } } while( 0 )
#else
#define glAssert( ... ) (void)0
#endif

void SetupRenderingOpenGL( zp_handle hWindow, zp_handle& hDC, zp_handle& hContext )
{
#ifdef ZP_WINDOWS
    BOOL ok;
    HWND wnd = static_cast<HWND>( hWindow );
    HDC dc = GetDC( wnd );
    hDC = dc;

    PIXELFORMATDESCRIPTOR pfd;
    zp_memset( &pfd, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

    pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    zp_int pixelFormatIndex = ChoosePixelFormat( dc, &pfd );
    ZP_ASSERT( pixelFormatIndex != 0, "" );

    ok = SetPixelFormat( dc, pixelFormatIndex, &pfd );
    ZP_ASSERT( ok, "" );

    HGLRC glContext = wglCreateContext( dc );
    ok = wglMakeCurrent( dc, glContext );
    ZP_ASSERT( ok, "" );
#endif

    glewExperimental = true;
    GLenum r = glewInit();
    ZP_ASSERT( r == GLEW_OK, "" );
    zp_printfln( "Using GLEW: %s", glewGetString( GLEW_VERSION ) );

    GLint OpenGLVersion[ 2 ];
    glGetIntegerv( GL_MAJOR_VERSION, &OpenGLVersion[ 0 ] );
    glGetIntegerv( GL_MINOR_VERSION, &OpenGLVersion[ 1 ] );

#ifdef ZP_WINDOWS
    if( WGLEW_ARB_create_context && OpenGLVersion[ 0 ] > 2 )
    {
        zp_printfln( "From GL:    %s", glGetString( GL_VERSION ) );

        int flags = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
#ifdef ZP_DEBUG
        flags |= WGL_CONTEXT_DEBUG_BIT_ARB;
#endif
        const int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, OpenGLVersion[ 0 ],
            WGL_CONTEXT_MINOR_VERSION_ARB, OpenGLVersion[ 1 ],
            WGL_CONTEXT_LAYER_PLANE_ARB, 0,
            WGL_CONTEXT_FLAGS_ARB, flags,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0, 0
        };

        wglMakeCurrent( ZP_NULL, ZP_NULL );
        wglDeleteContext( glContext );
        glContext = ZP_NULL;

        HGLRC arbContext = wglCreateContextAttribsARB( dc, 0, attribs );
        wglMakeCurrent( dc, arbContext );

        wglSwapIntervalEXT( 0 );

        hContext = arbContext;
    }
    else
#endif
    {
        hContext = glContext;
    }

    zp_printfln( "Using GL:   %s", glGetString( GL_VERSION ) );

#if ZP_DEBUG
    glEnable( GL_DEBUG_OUTPUT );
    glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
    if( OpenGLVersion[ 0 ] > 2 )
    {
        glDebugMessageCallback( _DebugOutputOpenGL, ZP_NULL );
        glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE );
        glDebugMessageControl( GL_DONT_CARE, GL_DEBUG_TYPE_PUSH_GROUP, GL_DONT_CARE, 0, 0, GL_FALSE );
        glDebugMessageControl( GL_DONT_CARE, GL_DEBUG_TYPE_POP_GROUP, GL_DONT_CARE, 0, 0, GL_FALSE );
    }
#endif

    glGenVertexArrays( zpVertexFormat_Count, g_vaos );
    glGenFramebuffers( NUM_FBO, g_fbo );

#if ZP_DEBUG
    zp_char buff[ 512 ];
    for( zp_uint i = 0; i < NUM_FBO; ++i )
    {
        zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), ZP_ARRAY_SIZE( buff ), "FBO %d", i );
        glBindFramebuffer( GL_FRAMEBUFFER, g_fbo[ i ] );
        glObjectLabel( GL_FRAMEBUFFER, g_fbo[ i ], -1, buff );
    }
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
#endif

#ifdef ZP_USE_PROFILER
    for( zp_uint i = 0; i < NUM_QUERIES; ++i )
    {
        glGenQueries( ZP_ARRAY_SIZE( g_queries[ i ].q ), g_queries[ i ].q );

        // perform dummy sample for next frame to remove GL errors
        glBeginQuery( GL_TIME_ELAPSED, g_queries[ i ].timeElapsed );
        glBeginQuery( GL_PRIMITIVES_GENERATED, g_queries[ i ].primitivesGenerated );
        glBeginQuery( GL_SAMPLES_PASSED, g_queries[ i ].samplesPassed );

#if ZP_DEBUG
        zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), ZP_ARRAY_SIZE( buff ), "Time Elapsed %d", i );
        glObjectLabel( GL_QUERY, g_queries[ i ].timeElapsed, -1, buff );

        zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), ZP_ARRAY_SIZE( buff ), "Primitives Generated %d", i );
        glObjectLabel( GL_QUERY, g_queries[ i ].primitivesGenerated, -1, buff );
        
        zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), ZP_ARRAY_SIZE( buff ), "Samples Passed %d", i );
        glObjectLabel( GL_QUERY, g_queries[ i ].samplesPassed, -1, buff );
#endif

        glEndQuery( GL_TIME_ELAPSED );
        glEndQuery( GL_PRIMITIVES_GENERATED );
        glEndQuery( GL_SAMPLES_PASSED );
    }
#endif

    glEnable( GL_MULTISAMPLE );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBlendEquation( GL_FUNC_ADD );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_GEQUAL );
    glDepthMask( GL_TRUE );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );

    glClipControl( GL_LOWER_LEFT, GL_ZERO_TO_ONE );

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    const zp_char* vertVC = R"GLSL(
in float4 vertex;                      
in fixed4 color;                        
uniform float4 _Color;               
out float4 fragmentColor;               
void main()                                
{                                          
    gl_Position = mul( _ObjectToWorld, vertex );  
    fragmentColor = color * _Color;                  
}                                          
)GLSL";
    const zp_char* fragVC = R"GLSL(
in float4 fragmentColor;    
out float4 outColor;        
void main()                    
{                              
    //gl_FragColor = fragmentColor;   
    outColor = fragmentColor;
}                              
)GLSL";

    const zp_char* vertVCU = R"GLSL(
in float4 vertex;                          
in fixed4 color;                            
in float2 uv0;                        
uniform float4 _Color;               
Sampler2D( _MainTex );                            
out float4 fragmentColor;                  
out float2 uv;                             
void main()                                    
{                                             
    gl_Position = mul( _ObjectToWorld, vertex );
    fragmentColor = color * _Color;                     
    uv = TransformTex( _MainTex, uv0 );                             
}                                              
)GLSL";
    const zp_char* fragVCU = R"GLSL(
Sampler2D( _MainTex );                
in float4 fragmentColor;              
in float2 uv;                         
out float4 outColor;             
void main()                           
{                                     
    //gl_FragColor = fragmentColor * tex2D( _MainTex, uv );
    outColor = fragmentColor * texture( _MainTex, uv );
}                                                      
)GLSL";

    zpCreateShaderDesc vc = {};
    vc.shaderName = "fragVC";
    vc.vertexShaderSrc = vertVC;
    vc.vertexShaderLength = zp_strlen( vertVC );
    vc.fragmentShaderSrc = fragVC;
    vc.fragmentShaderLength = zp_strlen( fragVC );

    zpCreateShaderDesc vcu = {};
    vcu.shaderName = "fragVCU";
    vcu.vertexShaderSrc = vertVCU;
    vcu.vertexShaderLength = zp_strlen( vertVCU );
    vcu.fragmentShaderSrc = fragVCU;
    vcu.fragmentShaderLength = zp_strlen( fragVCU );

    CreateShaderOpenGL( &vc, g_shaderVC );
    CreateShaderOpenGL( &vcu, g_shaderVCU );
}

void TeardownRenderingOpenGL( zp_handle hContext )
{
    DestroyShaderOpenGL( g_shaderVC );
    DestroyShaderOpenGL( g_shaderVCU );

    glDeleteVertexArrays( zpVertexFormat_Count, g_vaos );
    glDeleteFramebuffers( NUM_FBO, g_fbo );

#ifdef ZP_USE_PROFILER
    for( zp_uint i = 0; i < NUM_QUERIES; ++i )
    {
        glDeleteQueries( ZP_ARRAY_SIZE( g_queries[ i ].q ), g_queries[ i ].q );
    }
#endif
    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( ZP_NULL, ZP_NULL );
    wglDeleteContext( context );
}

void SetupCameraOpenGL( zpCamera* camera )
{

}

void ProcessRenderCommandOpenGL( const void* cmd, zp_size_t size )
{
    zp_size_t position = 0;

    const zp_size_t length = size;
    const zp_byte* data = static_cast<const zp_byte*>( cmd );

    while( position < length )
    {
        const void* ptr = data + position;
        const zpRenderCommandHeader* type = static_cast<const zpRenderCommandHeader*>( ptr );

        switch( type->type )
        {
            case ZP_RENDER_COMMAND_NOOP:
            {
                position += sizeof( zpRenderCommandHeader );
            } break;

            case ZP_RENDER_COMMAND_PUSH_MARKER:
            {
                const zpRenderCommandPushMarker* cmd = static_cast<const zpRenderCommandPushMarker*>( ptr );

                glMarkerPush( cmd->marker, static_cast<GLsizei>( cmd->length ) );

                position += sizeof( zpRenderCommandPushMarker );
            } break;

            case ZP_RENDER_COMMAND_POP_MARKER:
            {
                glMarkerPop();

                position += sizeof( zpRenderCommandPopMarker );
            } break;

            case ZP_RENDER_COMMAND_SET_VIEWPORT:
            {
                const zpRenderCommandSetViewport* cmd = static_cast<const zpRenderCommandSetViewport*>( ptr );
                
                glMarkerBlock( "Set Viewport" );
                glViewport( cmd->viewport.topX,
                            cmd->viewport.topY,
                            cmd->viewport.width,
                            cmd->viewport.height );
                //glDepthRangef( cmd->viewport.minDepth, cmd->viewport.maxDepth );

                position += sizeof( zpRenderCommandSetViewport );
            } break;

            case ZP_RENDER_COMMAND_SET_SCISSOR_RECT:
            {
                const zpRenderCommandSetScissorRect* cmd = static_cast<const zpRenderCommandSetScissorRect*>( ptr );
               
                glMarkerBlock( "Set Scissor Rect" );
                glScissor( cmd->scissorRect.x,
                           cmd->scissorRect.y,
                           cmd->scissorRect.width,
                           cmd->scissorRect.height );
                
                position += sizeof( zpRenderCommandSetScissorRect );
            } break;


            case ZP_RENDER_COMMAND_SET_BLEND_STATE:
            {
                const zpRenderCommandSetBlendState* cmd = static_cast<const zpRenderCommandSetBlendState*>( ptr );

                glMarkerBlock( "Set Blend State" );
                if( cmd->blendState.blendEnabled )
                {
                    glEnable( GL_BLEND );

                    if( cmd->blendState.separateBlendStates )
                    {
                        for( zp_uint i = 0; i < cmd->blendState.numBlendStates; ++i )
                        {
                            glBlendFuncSeparatei( i, _BlendModeToMode( cmd->blendState.blendStates[ i ].srcColorBlendMode ), _BlendModeToMode( cmd->blendState.blendStates[ i ].dstColorBlendMode ), _BlendModeToMode( cmd->blendState.blendStates[ i ].srcAlphaBlendMode ), _BlendModeToMode( cmd->blendState.blendStates[ i ].dstAlphaBlendMode ) );
                            glBlendEquationSeparatei( i, _BlendOperationToMode( cmd->blendState.blendStates[ i ].colorBlendOp ), _BlendOperationToMode( cmd->blendState.blendStates[ i ].alphaBlendOp ) );
                        }
                    }
                    else
                    {
                        glBlendFuncSeparate( _BlendModeToMode( cmd->blendState.blendStates[ 0 ].srcColorBlendMode ), _BlendModeToMode( cmd->blendState.blendStates[ 0 ].dstColorBlendMode ), _BlendModeToMode( cmd->blendState.blendStates[ 0 ].srcAlphaBlendMode ), _BlendModeToMode( cmd->blendState.blendStates[ 0 ].dstAlphaBlendMode ) );
                        glBlendEquationSeparate( _BlendOperationToMode( cmd->blendState.blendStates[ 0 ].colorBlendOp ), _BlendOperationToMode( cmd->blendState.blendStates[ 0 ].alphaBlendOp ) );
                    }
                }
                else
                {
                    glDisable( GL_BLEND );
                }

                position += sizeof( zpRenderCommandSetBlendState );
            } break;
            
            case ZP_RENDER_COMMAND_SET_DEPTH_STATE:
            {
                const zpRenderCommandSetDepthState* cmd = static_cast<const zpRenderCommandSetDepthState*>( ptr );

                glMarkerBlock( "Set Depth State" );
                if( cmd->depthState.compareFunc == ZP_COMPARE_FUNCTION_DISABLED )
                {
                    glDisable( GL_DEPTH_TEST );
                }
                else
                {
                    glEnable( GL_DEPTH_TEST );
                    glDepthFunc( _CompareFunctionToMode( cmd->depthState.compareFunc ) );
                }

                glDepthMask( cmd->depthState.writeEnabled ? GL_TRUE : GL_FALSE );

                position += sizeof( zpRenderCommandSetDepthState );
            } break;
            
            case ZP_RENDER_COMMAND_SET_STENCIL_STATE:
            {
                const zpRenderCommandSetStencilState* cmd = static_cast<const zpRenderCommandSetStencilState*>( ptr );

                glMarkerBlock( "Set Stencil State" );
                if( cmd->stencilState.stencilEnabled )
                {
                    glEnable( GL_STENCIL );

                    glStencilMaskSeparate( GL_FRONT, cmd->stencilState.writeMaskFront );
                    glStencilFuncSeparate( GL_FRONT, _CompareFunctionToMode( cmd->stencilState.compareFuncFront ), cmd->stencilState.reference, cmd->stencilState.readMaskFront );
                    glStencilOpSeparate( GL_FRONT, _StencilOperationToMode( cmd->stencilState.failOpFront ), _StencilOperationToMode( cmd->stencilState.zFailOpFront ), _StencilOperationToMode( cmd->stencilState.passOpFront ) );

                    glStencilMaskSeparate( GL_BACK, cmd->stencilState.writeMaskBack );
                    glStencilFuncSeparate( GL_BACK, _CompareFunctionToMode( cmd->stencilState.compareFuncBack ), cmd->stencilState.reference, cmd->stencilState.readMaskBack );
                    glStencilOpSeparate( GL_BACK, _StencilOperationToMode( cmd->stencilState.failOpBack ), _StencilOperationToMode( cmd->stencilState.zFailOpBack ), _StencilOperationToMode( cmd->stencilState.passOpBack ) );
                }
                else
                {
                    glDisable( GL_STENCIL );
                }

                position += sizeof( zpRenderCommandSetStencilState );
            } break;
            
            case ZP_RENDER_COMMAND_SET_RASTER_STATE:
            {
                const zpRenderCommandSetRasterState* cmd = static_cast<const zpRenderCommandSetRasterState*>( ptr );

                glMarkerBlock( "Set Raster State" );
                
                glPolygonMode( GL_FRONT_AND_BACK, _FillModeToMode( cmd->rasterState.fillMode ) );

                if( cmd->rasterState.cullingMode == ZP_CULL_MODE_OFF )
                {
                    glDisable( GL_CULL_FACE );
                }
                else
                {
                    glEnable( GL_CULL_FACE );
                    glCullFace( _CullModeToMode( cmd->rasterState.cullingMode ) );
                }

                glFrontFace( _FrontFaceToMode( cmd->rasterState.frontFace ) );
                glPolygonOffset( cmd->rasterState.depthOffsetFactor, cmd->rasterState.depthOffsetUnits );

                position += sizeof( zpRenderCommandSetRasterState );
            } break;

            case ZP_RENDER_COMMAND_CLEAR_COLOR_DEPTH_STENCIL:
            {
                const zpRenderCommandClear* cmd = static_cast<const zpRenderCommandClear*>( ptr );

                glMarkerBlock( "Clear Depth Stencil" );
                glClearColor( cmd->color.r,
                              cmd->color.g,
                              cmd->color.b,
                              cmd->color.a );
                glClearDepth( cmd->depth );
                glClearStencil( cmd->stencil );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

                position += sizeof( zpRenderCommandClear );
            } break;

            case ZP_RENDER_COMMAND_CLEAR_COLOR:
            {
                const zpRenderCommandClearColor* cmd = static_cast<const zpRenderCommandClearColor*>( ptr );

                glMarkerBlock( "Clear Color" );
                glClearColor( cmd->color.r,
                              cmd->color.g,
                              cmd->color.b,
                              cmd->color.a );
                glClear( GL_COLOR_BUFFER_BIT );

                position += sizeof( zpRenderCommandClearColor );
            } break;

            case ZP_RENDER_COMMAND_CLEAR_COLOR_DEPTH:
            {
                const zpRenderCommandClearColorDepth* cmd = static_cast<const zpRenderCommandClearColorDepth*>( ptr );

                glMarkerBlock( "Clear Color Depth" );
                glClearColor( cmd->color.r,
                              cmd->color.g,
                              cmd->color.b,
                              cmd->color.a );
                glClearDepth( cmd->depth );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

                position += sizeof( zpRenderCommandClearColorDepth );
            } break;

            case ZP_RENDER_COMMAND_CLEAR_DEPTH_STENCIL:
            {
                const zpRenderCommandClearDepthStencil* cmd = static_cast<const zpRenderCommandClearDepthStencil*>( ptr );

                glMarkerBlock( "Clear Depth Stencil" );
                glClearDepth( cmd->depth );
                glClearStencil( cmd->stencil );
                glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

                position += sizeof( zpRenderCommandClearDepthStencil );
            } break;

            case ZP_RENDER_COMMAND_CLEAR_DEPTH:
            {
                const zpRenderCommandClearDepth* cmd = static_cast<const zpRenderCommandClearDepth*>( ptr );

                glMarkerBlock( "Clear Depth" );
                glClearDepth( cmd->depth );
                glClear( GL_DEPTH_BUFFER_BIT );

                position += sizeof( zpRenderCommandClearDepth );
            } break;

            case ZP_RENDER_COMMAND_CLEAR_STENCIL:
            {
                const zpRenderCommandClearStencil* cmd = static_cast<const zpRenderCommandClearStencil*>( ptr );

                glMarkerBlock( "Clear Stencil" );
                glClearStencil( cmd->stencil );
                glClear( GL_STENCIL_BUFFER_BIT );

                position += sizeof( zpRenderCommandClearStencil );
            } break;

            case ZP_RENDER_COMMAND_SET_RT_COLOR:
            {
                const zpRenderCommandSetRenderTargetColor* cmd = static_cast<const zpRenderCommandSetRenderTargetColor*>( ptr );

                glMarkerBlock( "Set RT Color" );

                if( cmd->rtColor.renderTarget )
                {
                    GLuint colorTexture = cmd->rtColor.renderTarget->texture.index;
                    GLenum colorTarget = _TextureDimensionToTarget( cmd->rtColor.renderTarget->desc.textureDimension );
                    
                    GLenum colorAttachment = _DisplayFormatToAttachment( cmd->rtColor.renderTarget->desc.format );
                    const GLenum depthAttachment = GL_DEPTH_STENCIL_ATTACHMENT;

                    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, g_fbo[0] );
                    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, colorAttachment, colorTarget, colorTexture, 0 );
                    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, depthAttachment, colorTarget, 0, 0 );

                    glAssert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Failed to setup Framebuffer" );
                }
                else
                {
                    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
                }

                position += sizeof( zpRenderCommandSetRenderTargetColor );
            } break;

            case ZP_RENDER_COMMAND_SET_RT_COLOR_DEPTH:
            {
                const zpRenderCommandSetRenderTargetColorDepth* cmd = static_cast<const zpRenderCommandSetRenderTargetColorDepth*>( ptr );

                glMarkerBlock( "Set RT Color Depth" );

                if( cmd->rtColor.renderTarget && cmd->rtDepth.renderTarget )
                {
                    GLuint colorTexture = cmd->rtColor.renderTarget->texture.index;
                    GLuint depthTexture = cmd->rtDepth.renderTarget->texture.index;

                    GLenum colorTarget = _TextureDimensionToTarget( cmd->rtColor.renderTarget->desc.textureDimension );
                    GLenum depthTarget = _TextureDimensionToTarget( cmd->rtDepth.renderTarget->desc.textureDimension );

                    GLenum colorAttachment = _DisplayFormatToAttachment( cmd->rtColor.renderTarget->desc.format );
                    GLenum depthAttachment = _DisplayFormatToAttachment( cmd->rtDepth.renderTarget->desc.format );

                    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, g_fbo[0] );
                    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, colorAttachment, colorTarget, colorTexture, 0 );
                    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, depthAttachment, depthTarget, depthTexture, 0 );

                    glAssert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Failed to setup Framebuffer" );
                }
                else
                {
                    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
                }

                position += sizeof( zpRenderCommandSetRenderTargetColorDepth );
            } break;

            case ZP_RENDER_COMMAND_RESOLVE_ANTI_ALIAS_RT:
            {
                const zpRenderCommandResolveAntiAliasedRenderTarget* cmd = static_cast<const zpRenderCommandResolveAntiAliasedRenderTarget*>( ptr );

                glMarkerBlock( "Resolve Anti Alias RT" );

                GLuint srcTexture = cmd->rtAntiAliasSource.renderTarget->texture.index;
                GLenum srcTarget = _TextureDimensionToTarget( cmd->rtAntiAliasSource.renderTarget->desc.textureDimension );
                GLenum srcAttachment = _DisplayFormatToAttachment( cmd->rtAntiAliasSource.renderTarget->desc.format );

                GLint w = cmd->rtAntiAliasSource.renderTarget->desc.width;
                GLint h = cmd->rtAntiAliasSource.renderTarget->desc.height;

                GLuint trgFBO = 0;
                GLuint trgTexture = 0;
                GLenum trgTarget = srcTarget;
                GLenum trgAttachment = srcAttachment;
                
                if( cmd->rtTarget.renderTarget )
                {
                    trgFBO = g_fbo[ 1 ];
                    trgTexture = cmd->rtTarget.renderTarget->texture.index;
                    trgTarget = _TextureDimensionToTarget( cmd->rtTarget.renderTarget->desc.textureDimension );
                    trgAttachment = _DisplayFormatToAttachment( cmd->rtTarget.renderTarget->desc.format );
                }

                GLint prevDrawFramebuffer, prevReadFramebuffer;
                glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &prevDrawFramebuffer );
                glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &prevReadFramebuffer );

                GLbitfield mask = GL_COLOR_BUFFER_BIT;

                glBindFramebuffer( GL_READ_FRAMEBUFFER, g_fbo[ 0 ] );
                glFramebufferTexture2D( GL_READ_FRAMEBUFFER, srcAttachment, srcTarget, srcTexture, 0 );

                glBindFramebuffer( GL_DRAW_FRAMEBUFFER, trgFBO );
                if( trgFBO )
                {
                    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, trgAttachment, trgTarget, trgTexture, 0 );
                }

                glAssert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Failed to setup Framebuffer" );

                glBlitFramebuffer( 0, 0, w, h, 0, 0, w, h, mask, GL_LINEAR );

                glBindFramebuffer( GL_READ_FRAMEBUFFER, prevReadFramebuffer );
                glBindFramebuffer( GL_DRAW_FRAMEBUFFER, prevDrawFramebuffer );

                glAssert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Failed to setup Framebuffer" );

                position += sizeof( zpRenderCommandResolveAntiAliasedRenderTarget );
            } break;

            case ZP_RENDER_COMMAND_BLIT_RT:
            {
                const zpRenderCommandBlitRenderTexture* cmd = static_cast<const zpRenderCommandBlitRenderTexture*>( ptr );

                glMarkerBlock( "Blit RT" );

                GLint sw = 0;
                GLint sh = 0;
                GLint dw = 0;
                GLint dh = 0;

                GLuint srcTexture = 0;
                GLenum srcTarget = GL_TEXTURE_2D;
                GLenum srcAttachment = GL_COLOR_ATTACHMENT0;

                if( cmd->rtSrc.renderTarget )
                {
                    srcTexture = cmd->rtSrc.renderTarget->texture.index;
                    srcTarget = _TextureDimensionToTarget( cmd->rtSrc.renderTarget->desc.textureDimension );
                    srcAttachment = _DisplayFormatToAttachment( cmd->rtSrc.renderTarget->desc.format );

                    sw = cmd->rtSrc.renderTarget->desc.width;
                    sh = cmd->rtSrc.renderTarget->desc.height;
                }

                GLuint trgFBO = 0;
                GLuint trgTexture = 0;
                GLenum trgTarget = GL_TEXTURE_2D;
                GLenum trgAttachment = GL_COLOR_ATTACHMENT0;

                if( cmd->rtDst.renderTarget )
                {
                    trgFBO = g_fbo[ 1 ];
                    trgTexture = cmd->rtDst.renderTarget->texture.index;
                    trgTarget = _TextureDimensionToTarget( cmd->rtDst.renderTarget->desc.textureDimension );
                    trgAttachment = _DisplayFormatToAttachment( cmd->rtDst.renderTarget->desc.format );

                    dw = cmd->rtDst.renderTarget->desc.width;
                    dh = cmd->rtDst.renderTarget->desc.height;
                }

                if( cmd->rtSrc.renderTarget && !cmd->rtDst.renderTarget )
                {
                    dw = sw;
                    dh = sh;
                }
                else if( !cmd->rtSrc.renderTarget && cmd->rtDst.renderTarget )
                {
                    sw = dw;
                    sh = dh;
                }

                GLint prevDrawFramebuffer, prevReadFramebuffer;
                glGetIntegerv( GL_DRAW_FRAMEBUFFER_BINDING, &prevDrawFramebuffer );
                glGetIntegerv( GL_READ_FRAMEBUFFER_BINDING, &prevReadFramebuffer );

                GLbitfield mask = GL_COLOR_BUFFER_BIT;

                glBindFramebuffer( GL_READ_FRAMEBUFFER, g_fbo[ 0 ] );
                glFramebufferTexture2D( GL_READ_FRAMEBUFFER, srcAttachment, srcTarget, srcTexture, 0 );

                glBindFramebuffer( GL_DRAW_FRAMEBUFFER, trgFBO );
                if( trgFBO )
                {
                    glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, trgAttachment, trgTarget, trgTexture, 0 );
                }

                glAssert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Failed to setup Framebuffer" );

                glBlitFramebuffer( 0, 0, sw, sh, 0, 0, dw, dh, mask, GL_LINEAR );

                glBindFramebuffer( GL_READ_FRAMEBUFFER, prevReadFramebuffer );
                glBindFramebuffer( GL_DRAW_FRAMEBUFFER, prevDrawFramebuffer );

                glAssert( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "Failed to setup Framebuffer" );

                position += sizeof( zpRenderCommandBlitRenderTexture );
            } break;

            case ZP_RENDER_COMMAND_PRESENT:
            {
                const zpRenderCommandPresent* cmd = static_cast<const zpRenderCommandPresent*>( ptr );

                glMarkerBlock( "Present" );

                const HDC dc = static_cast<const HDC>( cmd->hDC );
                const HGLRC context = static_cast<const HGLRC>( cmd->hContext );

                wglMakeCurrent( dc, context );

                glFlush();

                SwapBuffers( dc );

                position += sizeof( zpRenderCommandPresent );
            } break;

            case ZP_RENDER_COMMAND_DRAW_MESH:
            {
                const zpRenderCommandDrawMesh* cmd = static_cast<const zpRenderCommandDrawMesh*>( ptr );
                glMarkerBlock( "Draw Mesh" );

                BindVertexFormatForRenderCommand( cmd );

                GLenum indexStride;
                switch( cmd->indexStride )
                {
                    case ZP_INDEX_STRIDE_BYTE:
                        // NOTE: byte is not well supported, fallback to ushort
                        //indexStride = GL_UNSIGNED_BYTE;
                        //break;

                    case ZP_INDEX_STRIDE_USHORT:
                        indexStride = GL_UNSIGNED_SHORT;
                        break;

                    case ZP_INDEX_STRIDE_UINT:
                        indexStride = GL_UNSIGNED_INT;
                        break;

                    default:
                        ZP_INVALID_CODE_PATH();
                        break;
                }

                GLenum mode = _TopologyToMode( cmd->topology );
                glDrawElements( mode,
                                static_cast<GLsizei>( cmd->indexCount ),
                                indexStride,
                                reinterpret_cast<void*>( cmd->indexOffset ) );


                UnbindVertexFormatForRenderCommand( cmd );

                position += sizeof( zpRenderCommandDrawMesh );
            } break;

            default:
            {
                position += sizeof( zpRenderCommandHeader );
            } break;
        }
    }
}

void PresentOpenGL( zp_handle hDC, zp_handle hContext )
{
    glMarkerBlock( "Present" );

    HDC dc = static_cast<HDC>( hDC );
    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( dc, context );
    
    glFlush();

    SwapBuffers( dc );
}

void CreateRenderBufferOpenGL( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Create Buffer" );

    buffer.size = size;
    buffer.stride = 0;
    buffer.bufferType = type;
    buffer.bindType = bindType;

    GLenum target = _BufferTypeToTarget( type );
    GLenum usage = _BufferBindTypeToUsage( bindType );

    glGenBuffers( 1, &buffer.buffer.index );
    glBindBuffer( target, buffer.buffer.index );
    glBufferData( target, size, data, usage );
    glBindBuffer( target, 0 );
}

void DestroyRenderBufferOpenGL( zpRenderBuffer& buffer )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Destroy Buffer" );

    glDeleteBuffers( 1, &buffer.buffer.index );
    buffer.buffer.index = 0;
}

void SetRenderBufferDataOpenGL( const zpRenderBuffer& buffer, const void* data, zp_size_t offset, zp_size_t length )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Set Buffer Data" );
    GLenum target = _BufferTypeToTarget( buffer.bufferType );

    glBindBuffer( target, buffer.buffer.index );
    glBufferSubData( target, offset, length, data );
    glBindBuffer( target, 0 );
}

void CreateTextureOpenGL( zpCreateTextureDesc* desc, zpTexture& texture )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Create Texture" );

    GLenum target = _TextureDimensionToTarget( desc->desc.textureDimension );
    GLint internalFormat = _DisplayFormatToInternalFormat( desc->desc.format );
    GLenum format = _DisplayFormatToFormat( desc->desc.format );
    GLenum type = _DisplayFormatToDataType( desc->desc.format );

    texture.desc = desc->desc;

    glGenTextures( 1, &texture.texture.index );
    glBindTexture( target, texture.texture.index );

    if( !_IsAntiAliased( desc->desc.textureDimension ) )
    {
        glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

        glTexParameteri( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

        glTexParameterf( target, GL_TEXTURE_LOD_BIAS, desc->desc.lodBias );
        glTexParameteri( target, GL_TEXTURE_BASE_LEVEL, desc->desc.minMipMap );

        if( desc->desc.mipMapCount >= 0 )
        {
            glTexParameteri( target, GL_TEXTURE_MAX_LEVEL, desc->desc.mipMapCount );
        }
    }

#if ZP_DEBUG
    GLsizei len = static_cast<GLsizei>( zp_strlen( desc->textureName ) );
    glObjectLabel( GL_TEXTURE, texture.texture.index, len, desc->textureName );
#endif

    zp_bool isCompressedFormat = _IsCompressedDisplayFormat( desc->desc.format );
    if( isCompressedFormat )
    {
        ZP_ASSERT( desc->desc.multisampleCount == 0, "Multisample compressed textures are unsupported" );

        zp_uint blockSize = ( format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ) ? 8 : 16;
        zp_uint offset = 0;
        GLsizei imageSize = 0;

        switch( desc->desc.textureDimension )
        {
            case ZP_TEXTURE_DIMENSION_1D:
                imageSize = _GetCompressedImageSize( desc->desc.width, 1, 1, blockSize );
                glCompressedTexImage1D( target, 0, internalFormat, desc->desc.width, 0, imageSize, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_1D_ARRAY:
                imageSize = _GetCompressedImageSize( desc->desc.width, desc->desc.height, 1, blockSize );
                glCompressedTexImage2D( target, 0, internalFormat, desc->desc.width, desc->desc.depth, 0, imageSize, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D:
                imageSize = _GetCompressedImageSize( desc->desc.width, desc->desc.height, 1, blockSize );
                glCompressedTexImage2D( target, 0, internalFormat, desc->desc.width, desc->desc.height, 0, imageSize, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D_ARRAY:
                imageSize = _GetCompressedImageSize( desc->desc.width, desc->desc.height, desc->desc.depth, blockSize );
                glCompressedTexImage3D( target, 0, internalFormat, desc->desc.width, desc->desc.height, desc->desc.depth, 0, imageSize, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE:
            case ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE_ARRAY:
                ZP_INVALID_CODE_PATH();
                break;
            case ZP_TEXTURE_DIMENSION_3D:
                imageSize = _GetCompressedImageSize( desc->desc.width, desc->desc.height, desc->desc.depth, blockSize );
                glCompressedTexImage3D( target, 0, internalFormat, desc->desc.width, desc->desc.height, desc->desc.depth, 0, imageSize, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_CUBE_MAP:
                ZP_INVALID_CODE_PATH();
                break;
            default:
                ZP_INVALID_CODE_PATH();
                break;
        }
    }
    else
    {
        switch( desc->desc.textureDimension )
        {
            case ZP_TEXTURE_DIMENSION_1D:
                glTexImage1D( target, 0, internalFormat, desc->desc.width, 0, format, type, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_1D_ARRAY:
                glTexImage2D( target, 0, internalFormat, desc->desc.width, desc->desc.depth, 0, format, type, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D:
                glTexImage2D( target, 0, internalFormat, desc->desc.width, desc->desc.height, 0, format, type, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D_ARRAY:
                glTexImage3D( target, 0, internalFormat, desc->desc.width, desc->desc.height, desc->desc.depth, 0, format, type, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE:
                glTexImage2DMultisample( target, desc->desc.multisampleCount, internalFormat, desc->desc.width, desc->desc.height, GL_TRUE );
                break;
            case ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE_ARRAY:
                glTexImage3DMultisample( target, desc->desc.multisampleCount, internalFormat, desc->desc.width, desc->desc.height, desc->desc.depth, GL_TRUE );
                break;
            case ZP_TEXTURE_DIMENSION_3D:
                glTexImage3D( target, 0, internalFormat, desc->desc.width, desc->desc.height, desc->desc.depth, 0, format, type, desc->pixels );
                break;
            case ZP_TEXTURE_DIMENSION_CUBE_MAP:
                ZP_INVALID_CODE_PATH();
                break;
            default:
                ZP_INVALID_CODE_PATH();
                break;
        }
    }

    if( !_IsAntiAliased( desc->desc.textureDimension ) )
    {
        glGenerateMipmap( target );
    }

    glBindTexture( target, 0 );
}

void DestroyTextureOpenGL( zpTexture& texture )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Destroy Texture" );
    
    glDeleteTextures( 1, &texture.texture.index );
    texture.texture.index = 0;
}

void CreateShaderOpenGL( zpCreateShaderDesc* desc, zpShader& shader )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Create Shader" );

    shader = {};

    const zp_char* versionHeader = R"GLSL(
#version 330 core
)GLSL";

    const zp_char* debuggingEnabledHeader = R"GLSL(
#pragma optimization(off)
#pragma debug(on)
)GLSL";

    const zp_char* hlslToGlslHeader = R"GLSL(
#define tex2D( sampler, uv )      texture( sampler, uv )
#define BeginCBuffer( name )
#define EndCBuffer()
#define mul( a, b )               ( (a) * (b) )
#define float2                    highp vec2
#define float3                    highp vec3
#define float4                    highp vec4
#define float4x4                  highp mat4
#define half                      mediump float
#define half2                     mediump vec2
#define half3                     mediump vec3
#define half4                     mediump vec4
#define fixed                     lowp float
#define fixed2                    lowp vec2
#define fixed3                    lowp vec3
#define fixed4                    lowp vec4
#define TransformTex( name, uv )  ( (uv) * name##_ST.xy + name##_ST.zw )
#define Sampler2D( name )         uniform sampler2D name; uniform float4 name##_ST
)GLSL";

    const zp_char* commonHeader = R"GLSL(
BeginCBuffer( PerFrame )
    uniform float4 _Time;
EndCBuffer()

BeginCBuffer( PerDrawCall )
    uniform float4x4 _ObjectToWorld;
EndCBuffer()

BeginCBuffer( Camera )
    uniform float4x4 _ViewProjection;
    uniform float4x4 _InvViewPorjection;
    uniform float4 _CameraUp;
    uniform float4 _CameraLookTo;
    uniform float4 _CameraPosition;
    uniform float4 _CameraData;
EndCBuffer()

float4 ObjectToClipSpace( in float4 vertex )
{
    return mul( _ViewProjection, mul( _ObjectToWorld, vertex ) );
}
)GLSL";

    const zp_char* vsHeader = R"GLSL(
#define VERTEX
)GLSL";

    const zp_char* psHeader = R"GLSL(
#define FRAGMENT
)GLSL";

    const zp_char* gsHeader = R"GLSL(
#define GEOMETRY
)GLSL";

    GLint result;
    GLint infoLength;
    zp_char buffer[ 1024 ];

    // vertex shader
    if( desc->vertexShaderSrc )
    {
        const zp_char* vs[] =
        {
            versionHeader,
            debuggingEnabledHeader,
            hlslToGlslHeader,
            commonHeader,
            vsHeader,
            static_cast<const zp_char*>( desc->vertexShaderSrc ) + desc->vertexShaderOffset
        };

        shader.vertexShader.index = glCreateShader( GL_VERTEX_SHADER );

        glShaderSource( shader.vertexShader.index, ZP_ARRAY_SIZE( vs ), vs, ZP_NULL );
        glCompileShader( shader.vertexShader.index );

        glGetShaderiv( shader.vertexShader.index, GL_COMPILE_STATUS, &result );
        if( result != GL_TRUE )
        {
            glGetShaderiv( shader.vertexShader.index, GL_INFO_LOG_LENGTH, &infoLength );
            if( infoLength )
            {
                glGetShaderInfoLog( shader.vertexShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
                zp_printfln( "Vertex Shader Error: %s", buffer );
            }

            glDeleteShader( shader.vertexShader.index );

            shader.vertexShader.index = 0;
        }
#if ZP_DEBUG
        else
        {
            zp_char buff[ 255 ];
            GLsizei len = static_cast<GLsizei>( zp_strlen( desc->shaderName ) );
            len = zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), len + 5, "%s.vert", desc->shaderName );

            glObjectLabel( GL_SHADER, shader.vertexShader.index, len, buff );
        }
#endif
    }

    // fragment shader
    if( desc->fragmentShaderSrc )
    {
        const zp_char* ps[] =
        {
            versionHeader,
            debuggingEnabledHeader,
            hlslToGlslHeader,
            commonHeader,
            psHeader,
            static_cast<const zp_char*>( desc->fragmentShaderSrc ) + desc->fragmentShaderOffset
        };

        shader.fragmentShader.index = glCreateShader( GL_FRAGMENT_SHADER );

        glShaderSource( shader.fragmentShader.index, ZP_ARRAY_SIZE( ps ), ps, ZP_NULL );
        glCompileShader( shader.fragmentShader.index );

        glGetShaderiv( shader.fragmentShader.index, GL_COMPILE_STATUS, &result );
        if( result != GL_TRUE )
        {
            glGetShaderiv( shader.fragmentShader.index, GL_INFO_LOG_LENGTH, &infoLength );
            if( infoLength )
            {
                glGetShaderInfoLog( shader.fragmentShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
                zp_printfln( "Fragment Shader Error: %s", buffer );
            }

            glDeleteShader( shader.fragmentShader.index );

            shader.fragmentShader.index = 0;
        }
#if ZP_DEBUG
        else
        {
            zp_char buff[ 255 ];
            GLsizei len = static_cast<GLsizei>( zp_strlen( desc->shaderName ) );
            len = zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), len + 5, "%s.frag", desc->shaderName );

            glObjectLabel( GL_SHADER, shader.fragmentShader.index, len, buff );
        }
#endif
    }

    // geometry shader
    if( desc->geometryShaderSrc )
    {
        const zp_char* gs[] =
        {
            versionHeader,
            debuggingEnabledHeader,
            hlslToGlslHeader,
            commonHeader,
            gsHeader,
            static_cast<const zp_char*>( desc->geometryShaderSrc ) + desc->geometryShaderOffset
        };

        shader.geometryShader.index = glCreateShader( GL_GEOMETRY_SHADER );

        glShaderSource( shader.geometryShader.index, ZP_ARRAY_SIZE( gs ), gs, ZP_NULL );
        glCompileShader( shader.geometryShader.index );

        glGetShaderiv( shader.geometryShader.index, GL_COMPILE_STATUS, &result );
        if( result != GL_TRUE )
        {
            glGetShaderiv( shader.geometryShader.index, GL_INFO_LOG_LENGTH, &infoLength );
            if( infoLength )
            {
                glGetShaderInfoLog( shader.geometryShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
                zp_printfln( "Geometry Shader Error: %s", buffer );
            }

            glDeleteShader( shader.geometryShader.index );

            shader.geometryShader.index = 0;
        }
#if ZP_DEBUG
        else
        {
            zp_char buff[ 255 ];
            GLsizei len = static_cast<GLsizei>( zp_strlen( desc->shaderName ) );
            len = zp_snprintf( buff, ZP_ARRAY_SIZE( buff ), len + 5, "%s.geom", desc->shaderName );

            glObjectLabel( GL_SHADER, shader.geometryShader.index, len, buff );
        }
#endif
    }

    // create program
    shader.programShader.index = glCreateProgram();

    if( shader.vertexShader.index )   glAttachShader( shader.programShader.index, shader.vertexShader.index );
    if( shader.fragmentShader.index ) glAttachShader( shader.programShader.index, shader.fragmentShader.index );
    if( shader.geometryShader.index ) glAttachShader( shader.programShader.index, shader.geometryShader.index );

    // link program
    glLinkProgram( shader.programShader.index );

    // print link erros
    glGetProgramiv( shader.programShader.index, GL_LINK_STATUS, &result );
    if( result != GL_TRUE )
    {
        glGetProgramiv( shader.programShader.index, GL_INFO_LOG_LENGTH, &infoLength );
        if( infoLength )
        {
            glGetProgramInfoLog( shader.programShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
            zp_printfln( "Program Link Error: %s", buffer );
        }

        glDeleteProgram( shader.programShader.index );

        shader.programShader.index = 0;
    }
#if ZP_DEBUG
    else
    {
        GLsizei len = static_cast<GLsizei>( zp_strlen( desc->shaderName ) );
        glObjectLabel( GL_PROGRAM, shader.programShader.index, len, desc->shaderName );
    }
#endif

    // validate program
    glValidateProgram( shader.programShader.index );

    // print validation errors
    glGetProgramiv( shader.programShader.index, GL_VALIDATE_STATUS, &result );
    if( result != GL_TRUE )
    {
        glGetProgramiv( shader.programShader.index, GL_INFO_LOG_LENGTH, &infoLength );
        if( infoLength )
        {
            glGetProgramInfoLog( shader.programShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
            zp_printfln( "Program Validation Error: %s", buffer );
        }
    }

    // clean-up shaders
    if( shader.vertexShader.index )
    {
        glDetachShader( shader.programShader.index, shader.vertexShader.index );
        glDeleteShader( shader.vertexShader.index );
    }

    if( shader.fragmentShader.index )
    {
        glDetachShader( shader.programShader.index, shader.fragmentShader.index );
        glDeleteShader( shader.fragmentShader.index );
    }
    
    if( shader.geometryShader.index )
    {
        glDetachShader( shader.programShader.index, shader.geometryShader.index );
        glDeleteShader( shader.geometryShader.index );
    }

    shader.vertexShader.index = 0;
    shader.fragmentShader.index = 0;
    shader.geometryShader.index = 0;
}

void DestroyShaderOpenGL( zpShader& shader )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Destroy Shader" );

    if( shader.vertexShader.index )
    {
        glDeleteShader( shader.vertexShader.index );
        shader.vertexShader.index = 0;
    }

    if( shader.fragmentShader.index )
    {
        glDeleteShader( shader.fragmentShader.index );
        shader.fragmentShader.index = 0;
    }

    if( shader.geometryShader.index )
    {
        glDeleteShader( shader.geometryShader.index );
        shader.geometryShader.index = 0;
    }

    if( shader.programShader.index )
    {
        glDeleteProgram( shader.programShader.index );
        shader.programShader.index = 0;
    }
}

#ifdef ZP_USE_PROFILER
void BeginFrameOpenGL( zp_size_t frameIndex )
{
    zp_size_t sampleIndex = frameIndex % NUM_QUERIES;
    glBeginQuery( GL_TIME_ELAPSED, g_queries[ sampleIndex ].timeElapsed );
    glBeginQuery( GL_PRIMITIVES_GENERATED, g_queries[ sampleIndex ].primitivesGenerated );
    glBeginQuery( GL_SAMPLES_PASSED, g_queries[ sampleIndex ].samplesPassed );
}

void EndFrameOpenGL( zp_size_t frameIndex, zpRenderingStat& stat )
{
    glEndQuery( GL_TIME_ELAPSED );
    glEndQuery( GL_PRIMITIVES_GENERATED );
    glEndQuery( GL_SAMPLES_PASSED );

    zp_size_t sampleIndex = ( frameIndex + 1 ) % NUM_QUERIES;

    GLuint64 timeElapsed;
    GLuint64 primitviesGenerated;
    GLuint64 samplesPassed;
    glGetQueryObjectui64v( g_queries[ sampleIndex ].timeElapsed, GL_QUERY_RESULT_NO_WAIT, &timeElapsed );
    glGetQueryObjectui64v( g_queries[ sampleIndex ].primitivesGenerated, GL_QUERY_RESULT_NO_WAIT, &primitviesGenerated );
    glGetQueryObjectui64v( g_queries[ sampleIndex ].samplesPassed, GL_QUERY_RESULT_NO_WAIT, &samplesPassed );

    stat.frameIndex = frameIndex;
    stat.frameTime = timeElapsed;
    stat.primitiveCount = primitviesGenerated;
    stat.samplesPassed = samplesPassed;
}
#else
#define BeginFrameOpenGL( ... )     (void)0
#define EndFrameOpenGL( ... )       (void)0
#endif