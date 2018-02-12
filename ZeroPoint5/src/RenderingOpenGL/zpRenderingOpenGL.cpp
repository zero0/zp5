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
    static GLenum mapping[] =
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
    static GLenum mapping[] =
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

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpBufferType_Count );
    return mapping[ type ];
}

static ZP_FORCE_INLINE GLenum _BufferBindTypeToUsage( zpBufferBindType bindType )
{
    static GLenum mapping[] =
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
    static GLenum mapping[] =
    {
        0,
        GL_TEXTURE_1D,
        GL_TEXTURE_2D,
        GL_TEXTURE_3D,
        GL_TEXTURE_CUBE_MAP,
    };

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpTextureDimension_Count );
    return mapping[ textureDimenision ];
}

static ZP_FORCE_INLINE GLint _DisplayFormatToInternalFormat( zpDisplayFormat displayFormat )
{
    static GLint mapping[] =
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

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpDisplayFormat_Count );
    return mapping[ displayFormat ];
}

static ZP_FORCE_INLINE GLenum _DisplayFormatToFormat( zpDisplayFormat displayFormat )
{
    static GLint mapping[] =
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

        GL_RGB_INTEGER,
        GL_RGB_INTEGER,
        GL_RGB,

        // RGBA Components
        GL_RGBA_INTEGER,
        GL_RGBA_INTEGER,
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

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpDisplayFormat_Count );
    return mapping[ displayFormat ];
}

static ZP_FORCE_INLINE GLenum _DisplayFormatToDataType( zpDisplayFormat displayFormat )
{
    static GLenum mapping[] =
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

    ZP_STATIC_ASSERT( ( sizeof( mapping ) / sizeof( mapping[ 0 ] ) ) == zpDisplayFormat_Count );
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

static GLuint g_vaos[ zpVertexFormat_Count ];
static zpShader g_shaderVC;
static zpShader g_shaderVCU;
#ifdef ZP_USE_PROFILER
static glQuery g_queries[ 2 ];
#endif

static void BindVertexFormatForRenderCommand( const zpRenderingCommand* cmd )
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
    if( cmd->material.isValid() && cmd->material->shader.isValid() )
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
            glEnableVertexAttribArray( 5 );
            glVertexAttribPointer( 5, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 5 ] ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT:
            glEnableVertexAttribArray( 4 );
            glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 4 ] ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL:
            glEnableVertexAttribArray( 3 );
            glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 3 ] ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
            glEnableVertexAttribArray( 2 );
            glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 2 ] ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
            glEnableVertexAttribArray( 1 );
            glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 1 ] ) );
            glEnableVertexAttribArray( 0 );
            glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( vertexOffset + offsets[ 0 ] ) );
            break;
        default:
            break;
    }

    GLint otw = glGetUniformLocation( prog, "_ObjectToWorld" );
    glUniformMatrix4fv( otw, 1, GL_FALSE, cmd->transform.m );

    if( cmd->material.isValid() )
    {
        const zpMaterialPartColor* cb = cmd->material->colors.begin();
        const zpMaterialPartColor* ce = cmd->material->colors.end();
        for( ; cb != ce; ++cb )
        {
            GLint c = glGetUniformLocation( prog, cb->name.str() );
            if( c >= 0 )
            {
                glUniform4fv( c, 1, cb->color.rgba );
            }
        }

        const zpMaterialPartVector* vb = cmd->material->vectors.begin();
        const zpMaterialPartVector* ve = cmd->material->vectors.end();
        for( ; vb != ve; ++vb )
        {
            GLint c = glGetUniformLocation( prog, vb->name.str() );
            if( c >= 0 )
            {
                glUniform4fv( c, 1, vb->vector.m );
            }
        }

        const zpMaterialPartTexture* tb = cmd->material->textures.begin();
        const zpMaterialPartTexture* te = cmd->material->textures.end();
        for( ; tb != te; ++tb )
        {
            GLint c = glGetUniformLocation( prog, tb->name.str() );
            if( c >= 0 )
            {
                GLint i = static_cast<GLint>( te - tb );
                glActiveTexture( GL_TEXTURE0 + i );
                glBindTexture( GL_TEXTURE_2D, tb->texture->texture.index );
                glUniform1i( c, i );
            }
        }

        const zpMaterialPartMatrix* mb = cmd->material->matrices.begin();
        const zpMaterialPartMatrix* me = cmd->material->matrices.end();
        for( ; mb != me; ++mb )
        {
            GLint c = glGetUniformLocation( prog, mb->name.str() );
            if( c >= 0 )
            {
                glUniformMatrix4fv( c, 1, GL_FALSE, mb->matrix.m );
            }
        }
    }

    // TODO: update material to set these
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDepthFunc( GL_LEQUAL );
    glDepthMask( GL_TRUE );
}

static void UnbindVertexFormatForRenderCommand( const zpRenderingCommand* cmd )
{
    switch( cmd->vertexFormat )
    {
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT_UV2:
            glDisableVertexAttribArray( 5 );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT:
            glDisableVertexAttribArray( 4 );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL:
            glDisableVertexAttribArray( 3 );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
            glDisableVertexAttribArray( 2 );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
            glDisableVertexAttribArray( 1 );
            glDisableVertexAttribArray( 0 );
            break;
        default:
            break;
    }

    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );

    GLuint prog = 0;
    if( cmd->material.isValid() && cmd->material->shader.isValid() )
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

    if( cmd->material.isValid() )
    {
        const zpMaterialPartTexture* tb = cmd->material->textures.begin();
        const zpMaterialPartTexture* te = cmd->material->textures.end();
        for( ; tb != te; ++tb )
        {
            GLint c = glGetUniformLocation( prog, tb->name.str() );
            if( c >= 0 )
            {
                GLint i = static_cast<GLint>( te - tb );
                glActiveTexture( GL_TEXTURE0 + i );
                glBindTexture( GL_TEXTURE_2D, 0 );
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

#ifdef ZP_DEBUG
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

#ifdef ZP_USE_PROFILER
    glGenQueries( ZP_ARRAY_SIZE( g_queries[ 0 ].q ), g_queries[ 0 ].q );
    glGenQueries( ZP_ARRAY_SIZE( g_queries[ 1 ].q ), g_queries[ 1 ].q );

    // perform dummy sample for next frame to remove GL errors
    glBeginQuery( GL_TIME_ELAPSED, g_queries[ 1 ].timeElapsed );
    glBeginQuery( GL_PRIMITIVES_GENERATED, g_queries[ 1 ].primitivesGenerated );
    glBeginQuery( GL_SAMPLES_PASSED, g_queries[ 1 ].samplesPassed );

    glEndQuery( GL_TIME_ELAPSED );
    glEndQuery( GL_PRIMITIVES_GENERATED );
    glEndQuery( GL_SAMPLES_PASSED );
#endif

    glEnable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glDepthFunc( GL_LEQUAL );
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );

    const zp_char* vertVC = R"GLSL(
        layout(location = 0) in float4 vertex;                      
        layout(location = 1) in fixed4 color;                        
        uniform fixed4 _Color;               
        out fixed4 fragmentColor;               
        void main()                                
        {                                          
           gl_Position = mul( _ObjectToWorld, vertex );  
           fragmentColor = color;                  
        }                                          
        )GLSL";
    const zp_char* fragVC = R"GLSL(
        in fixed4 fragmentColor;    
        out lowp vec4 outColor;        
        void main()                    
        {                              
           gl_FragColor = fragmentColor;   
        }                              
        )GLSL";

    const zp_char* vertVCU = R"GLSL(
        layout(location = 0) in float4 vertex;                          
        layout(location = 1) in fixed4 color;                            
        layout(location = 2) in float2 texcoord;                        
        uniform fixed4 _Color;               
        Sampler2D( _MainTex );                            
        out fixed4 fragmentColor;                  
        out float2 uv;                             
        void main()                                    
        {                                             
            gl_Position = mul( _ObjectToWorld, vertex );
            fragmentColor = color * _Color;                     
            uv = TransformTex( _MainTex, texcoord );                             
        }                                              
        )GLSL";
    const zp_char* fragVCU = R"GLSL(
        Sampler2D( _MainTex );                
        in fixed4 fragmentColor;              
        in float2 uv;                         
        out lowp vec4 outColor;             
        void main()                           
        {                                     
            gl_FragColor = fragmentColor * tex2D( _MainTex, uv );
        }                                                      
        )GLSL";

    CreateShaderOpenGL( "fragVC", vertVC, fragVC, g_shaderVC );
    CreateShaderOpenGL( "fragVCU", vertVCU, fragVCU, g_shaderVCU );
}

void TeardownRenderingOpenGL( zp_handle hContext )
{
    glDeleteVertexArrays( zpVertexFormat_Count, g_vaos );

#ifdef ZP_USE_PROFILER
    glDeleteQueries( ZP_ARRAY_SIZE( g_queries[0].q ), g_queries[0].q );
    glDeleteQueries( ZP_ARRAY_SIZE( g_queries[1].q ), g_queries[1].q );
#endif
    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( ZP_NULL, ZP_NULL );
    wglDeleteContext( context );
}

void SetupCameraOpenGL( zpCamera* camera )
{

}

void ProcessRenderingCommandOpenGL( const zpRenderingCommand* cmd )
{
    switch( cmd->type )
    {
        case ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT:
        {
            glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Set Scissor Rect" );
            glScissor( cmd->scissorRect.x,
                       cmd->scissorRect.y,
                       cmd->scissorRect.width,
                       cmd->scissorRect.height );
        }
            break;

        case ZP_RENDERING_COMMNAD_SET_VIEWPORT:
        {
            glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Set Viewport" );
            glViewport( cmd->viewport.topX,
                        cmd->viewport.topY,
                        cmd->viewport.width,
                        cmd->viewport.height );
            glDepthRange( cmd->viewport.minDepth, cmd->viewport.maxDepth );
        }
            break;

        case ZP_RENDERING_COMMNAD_CLEAR:
        {
            glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Clear" );
            glClearColor( cmd->clearColor.r,
                          cmd->clearColor.g,
                          cmd->clearColor.b,
                          cmd->clearColor.a );
            glClearDepth( cmd->clearDepth );
            glClearStencil( cmd->clearStencil );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        }
            break;

        case ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE:
        {
            glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Draw Immediate" );
            
            BindVertexFormatForRenderCommand( cmd );

            GLenum mode = _TopologyToMode( cmd->topology );
            glDrawElements( mode,
                            static_cast<GLsizei>( cmd->indexCount ),
                            GL_UNSIGNED_SHORT,
                            reinterpret_cast<void*>( cmd->indexOffset ) );


            UnbindVertexFormatForRenderCommand( cmd );
        }
            break;

        case ZP_RENDERING_COMMNAD_DRAW_BUFFERED:
            break;

        case ZP_RENDERING_COMMNAD_DRAW_INSTANCED:
            break;

        default:
            ZP_INVALID_CODE_PATH();
            break;
    }
}

void PresentOpenGL( zp_handle hDC, zp_handle hContext )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Present" );

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

void CreateTextureOpenGL( const zp_char* textureName, zp_uint width, zp_uint height, zp_int mipMapCount, zpDisplayFormat displayFormat, zpTextureDimension textureDimension, zpTextureType textureType, const void* pixels, zpTexture& texture )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Create Texture" );

    GLenum target = _TextureDimensionToTarget( textureDimension );
    GLint internalFormat = _DisplayFormatToInternalFormat( displayFormat );
    GLenum format = _DisplayFormatToFormat( displayFormat );
    GLenum type = _DisplayFormatToDataType( displayFormat );

    texture.width = width;
    texture.height = height;
    texture.textureDimension = textureDimension;
    texture.type = textureType;
    texture.format = displayFormat;

    glGenTextures( 1, &texture.texture.index );
    glBindTexture( target, texture.texture.index );
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

#if ZP_DEBUG
    GLsizei len = static_cast<GLsizei>( zp_strlen( textureName ) );
    glObjectLabel( GL_TEXTURE, texture.texture.index, len, textureName );
#endif

    zp_bool isCompressedFormat = _IsCompressedDisplayFormat( displayFormat );
    if( isCompressedFormat )
    {
        zp_uint blockSize = ( format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ) ? 8 : 16;
        zp_uint offset = 0;
        GLsizei imageSize = 0;

        switch( textureDimension )
        {
            case ZP_TEXTURE_DIMENSION_1D:
                imageSize = ( ( width + 3 ) / 4 ) * blockSize;
                glCompressedTexImage1D( target, 0, internalFormat, width, 0, imageSize, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D:
                imageSize = ( ( width + 3 ) / 4 ) * ( ( height + 3 ) / 4 ) * blockSize;
                glCompressedTexImage2D( target, 0, internalFormat, width, height, 0, imageSize, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_3D:
                ZP_INVALID_CODE_PATH();
                glCompressedTexImage3D( target, 0, internalFormat, width, height, 0, 0, imageSize, pixels );
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
        switch( textureDimension )
        {
            case ZP_TEXTURE_DIMENSION_1D:
                glTexImage1D( target, 0, internalFormat, width, 0, format, type, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_2D:
                glTexImage2D( target, 0, internalFormat, width, height, 0, format, type, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_3D:
                ZP_INVALID_CODE_PATH();
                glTexImage3D( target, 0, internalFormat, width, height, 0, 0, format, type, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_CUBE_MAP:
                ZP_INVALID_CODE_PATH();
                break;
            default:
                ZP_INVALID_CODE_PATH();
                break;
        }
    }

    if( mipMapCount < 0 )
    {
        glGenerateMipmap( target );
    }

    glBindTexture( target, 0 );
}

void DestroyTextureOpenGL( zpTexture& texture )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Destroy Buffer" );
    
    glDeleteTextures( 1, &texture.texture.index );
    texture.texture.index = 0;
}

void CreateShaderOpenGL( const zp_char* shaderName, const zp_char* vertexShaderSource, const zp_char* fragmentShaderSource, zpShader& shader )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Create Shader" );

    zpShader empty = {};
    shader = empty;

    // TODO: add to method
    const zp_char* geometroyShaderSource = ZP_NULL;

    const zp_char* versionHeader = R"GLSL(
        #version 330 core
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
            uniform float4x4 _ViewPorjection;
            uniform float4x4 _InvViewPorjection;
            uniform float4 _CameraUp;
            uniform float4 _CameraLookTo;
            uniform float4 _CameraPosition;
            uniform float4 _CameraData;
        EndCBuffer()

        float4 ObjectToClipSpace( in float4 vertex )
        {
          return mul( _ViewPorjection, mul( _ObjectToWorld, vertex ) );
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
    zp_char buffer[ 512 ];

    // vertex shader
    if( vertexShaderSource )
    {
        const zp_char* vs[] =
        {
            versionHeader,
            commonHeader,
            vsHeader,
            vertexShaderSource
        };

        shader.vertexShader.index = glCreateShader( GL_VERTEX_SHADER );

        glShaderSource( shader.vertexShader.index, ZP_ARRAY_SIZE( vs ), vs, ZP_NULL );
        glCompileShader( shader.vertexShader.index );

        glGetShaderiv( shader.vertexShader.index, GL_COMPILE_STATUS, &result );
        glGetShaderiv( shader.vertexShader.index, GL_INFO_LOG_LENGTH, &infoLength );
        if( infoLength )
        {
            glGetShaderInfoLog( shader.vertexShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
            zp_printfln( "Vertex Shader Error: %s", buffer );
        }
    }

    // fragment shader
    if( fragmentShaderSource )
    {
        const zp_char* ps[] =
        {
            versionHeader,
            commonHeader,
            psHeader,
            fragmentShaderSource
        };

        shader.fragmentShader.index = glCreateShader( GL_FRAGMENT_SHADER );

        glShaderSource( shader.fragmentShader.index, ZP_ARRAY_SIZE( ps ), ps, ZP_NULL );
        glCompileShader( shader.fragmentShader.index );

        glGetShaderiv( shader.fragmentShader.index, GL_COMPILE_STATUS, &result );
        glGetShaderiv( shader.fragmentShader.index, GL_INFO_LOG_LENGTH, &infoLength );
        if( infoLength )
        {
            glGetShaderInfoLog( shader.fragmentShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
            zp_printfln( "Fragment Shader Error: %s", buffer );
        }
    }

    // geometry shader
    if( geometroyShaderSource )
    {
        const zp_char* gs[] =
        {
            versionHeader,
            commonHeader,
            gsHeader,
            geometroyShaderSource
        };

        shader.geometryShader.index = glCreateShader( GL_GEOMETRY_SHADER );

        glShaderSource( shader.geometryShader.index, ZP_ARRAY_SIZE( gs ), gs, ZP_NULL );
        glCompileShader( shader.geometryShader.index );

        glGetShaderiv( shader.geometryShader.index, GL_COMPILE_STATUS, &result );
        glGetShaderiv( shader.geometryShader.index, GL_INFO_LOG_LENGTH, &infoLength );
        if( infoLength )
        {
            glGetShaderInfoLog( shader.geometryShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
            zp_printfln( "Geometry Shader Error: %s", buffer );
        }
    }

    // link program
    shader.programShader.index = glCreateProgram();
    if( shader.vertexShader.index )   glAttachShader( shader.programShader.index, shader.vertexShader.index );
    if( shader.fragmentShader.index ) glAttachShader( shader.programShader.index, shader.fragmentShader.index );
    if( shader.geometryShader.index ) glAttachShader( shader.programShader.index, shader.geometryShader.index );

    glLinkProgram( shader.programShader.index );

    glGetProgramiv( shader.programShader.index, GL_LINK_STATUS, &result );
    glGetProgramiv( shader.programShader.index, GL_INFO_LOG_LENGTH, &infoLength );
    if( infoLength )
    {
        glGetProgramInfoLog( shader.programShader.index, ZP_ARRAY_SIZE( buffer ), ZP_NULL, buffer );
        zp_printfln( "Program Link Error: %s", buffer );
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

#if ZP_DEBUG
    GLsizei len = static_cast<GLsizei>( zp_strlen( shaderName ) );
    glObjectLabel( GL_PROGRAM, shader.programShader.index, len, shaderName );
#endif

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
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "Begin Frame" );

    zp_size_t sampleIndex = frameIndex % 2;
    glBeginQuery( GL_TIME_ELAPSED, g_queries[ sampleIndex ].timeElapsed );
    glBeginQuery( GL_PRIMITIVES_GENERATED, g_queries[ sampleIndex ].primitivesGenerated );
    glBeginQuery( GL_SAMPLES_PASSED, g_queries[ sampleIndex ].samplesPassed );
}

void EndFrameOpenGL( zp_size_t frameIndex, zpRenderingStat& stat )
{
    glDebugBlock( GL_DEBUG_SOURCE_APPLICATION, "End Frame" );

    glEndQuery( GL_TIME_ELAPSED );
    glEndQuery( GL_PRIMITIVES_GENERATED );
    glEndQuery( GL_SAMPLES_PASSED );

    zp_size_t sampleIndex = ( frameIndex + 1 ) % 2;

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