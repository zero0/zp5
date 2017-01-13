#include "Core\zpCore.h"
#include "zpRenderingOpenGL.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

#define GLEW_STATIC
#include <GL\glew.h>

ZP_FORCE_INLINE GLenum _TopologyToMode( zpTopology topology )
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

ZP_FORCE_INLINE GLenum _BufferTypeToTarget( zpBufferType type )
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

ZP_FORCE_INLINE GLenum _BufferBindTypeToUsage( zpBufferBindType bindType )
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

ZP_FORCE_INLINE GLenum _TextureDimensionToTarget( zpTextureDimension textureDimenision )
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

ZP_FORCE_INLINE GLint _DisplayFormatToInternalFormat( zpDisplayFormat displayFormat )
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

ZP_FORCE_INLINE GLenum _DisplayFormatToFormat( zpDisplayFormat displayFormat )
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

ZP_FORCE_INLINE GLenum _DisplayFormatToDataType( zpDisplayFormat displayFormat )
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

ZP_FORCE_INLINE zp_bool _IsCompressedDisplayFormat( zpDisplayFormat displayFormat )
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

static GLuint g_vaos[ zpVertexFormat_Count ];
static zpShader g_shaderVC;
static zpShader g_shaderVCU;

void BindVertexFormatForRenderCommand( zpRenderingCommand* cmd )
{
    static zp_int strides[] =
    {
        sizeof( zp_float ) * 0,
        sizeof( zp_float ) * ( 4 + 4 ),
        sizeof( zp_float ) * ( 4 + 4 + 2 ),
        sizeof( zp_float ) * ( 4 + 4 + 2 + 4 ),
        sizeof( zp_float ) * ( 4 + 4 + 2 + 4 + 4 ),
        sizeof( zp_float ) * ( 4 + 4 + 2 + 4 + 4 + 2 )
    };
    ZP_STATIC_ASSERT( ( sizeof( strides ) / sizeof( strides[ 0 ] ) ) == zpVertexFormat_Count );

    zp_int stride = strides[ cmd->vertexFormat ];
    
    glBindVertexArray( g_vaos[ cmd->vertexFormat ] );
    glBindBuffer( GL_ARRAY_BUFFER, cmd->vertexBuffer.bufferIndex );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, cmd->indexBuffer.bufferIndex );

    switch( cmd->vertexFormat )
    {
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT_UV2:
            glEnableVertexAttribArray( 5 );
            glVertexAttribPointer( 5, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( sizeof( zp_float ) * 18 ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL_TANGENT:
            glEnableVertexAttribArray( 4 );
            glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( sizeof( zp_float ) * 14 ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV_NORMAL:
            glEnableVertexAttribArray( 3 );
            glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( sizeof( zp_float ) * 10 ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
            glEnableVertexAttribArray( 2 );
            glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( sizeof( zp_float ) * 8 ) );
        case ZP_VERTEX_FORMAT_VERTEX_COLOR:
            glEnableVertexAttribArray( 1 );
            glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( sizeof( zp_float ) * 4 ) );
            glEnableVertexAttribArray( 0 );
            glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>( sizeof( zp_float ) * 0 ) );
            break;
        default:
            break;
    }
}

void UnbindVertexFormatForRenderCommand( zpRenderingCommand* cmd )
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
}

void SetupRenderingOpenGL( zp_handle hWindow, zp_handle& hDC, zp_handle& hContext )
{
#ifdef ZP_WINDOWS
    BOOL ok;
    HWND wnd = static_cast<HWND>( hWindow );
    HDC dc = GetDC( wnd );

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

    HGLRC context = wglCreateContext( dc );
    ok = wglMakeCurrent( dc, context );
    ZP_ASSERT( ok, "" );

    hDC = dc;
    hContext = context;
#endif

    glewExperimental = true;
    GLenum r = glewInit();
    ZP_ASSERT( r == GLEW_OK, "" );

    glGenVertexArrays( zpVertexFormat_Count, g_vaos );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    zp_printfln( "Using GLEW: %s", glewGetString( GLEW_VERSION ) );
    zp_printfln( "Using GL:   %s", glGetString( GL_VERSION ) );

    const zp_char* vertVC = "#version 330 core\n"
        "layout(location = 0) in vec4 position;"
        "layout(location = 1) in vec4 color;"
        "out vec4 fragmentColor;"
        "void main() {"
            "gl_Position = position;"
            "fragmentColor = color;"
        "}";
    const zp_char* fragVC = "#version 330 core\n"
        "in vec4 fragmentColor;"
        "out vec4 outColor;"
        "void main() {"
            "outColor = fragmentColor;"
        "}";

    const zp_char* vertVCU = "#version 330 core\n"
        "layout(location = 0) in vec4 position;"
        "layout(location = 1) in vec4 color;"
        "layout(location = 2) in vec2 texcoord;"
        "out vec4 fragmentColor;"
        "void main() {"
            "gl_Position = position;"
            "fragmentColor = color;"
        "}";
    const zp_char* fragVCU = "#version 330 core\n"
        "in vec4 fragmentColor;"
        "out vec4 outColor;"
        "void main() {"
            "outColor = fragmentColor;"
        "}";
    CreateShaderOpenGL( vertVC, fragVC, g_shaderVC );
    CreateShaderOpenGL( vertVCU, fragVCU, g_shaderVCU );
}

void TeardownRenderingOpenGL( zp_handle hContext )
{
    glDeleteVertexArrays( zpVertexFormat_Count, g_vaos );

    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( ZP_NULL, ZP_NULL );
    wglDeleteContext( context );
}

void ProcessRenderingCommandOpenGL( zpRenderingCommand* cmd )
{
    switch( cmd->type )
    {
        case ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT:
            glScissor( cmd->scissorRect.x,
                       cmd->scissorRect.y,
                       cmd->scissorRect.width,
                       cmd->scissorRect.height );
            break;

        case ZP_RENDERING_COMMNAD_SET_VIEWPORT:
            glViewport( cmd->viewport.topX,
                        cmd->viewport.topY,
                        cmd->viewport.width,
                        cmd->viewport.height );
            break;

        case ZP_RENDERING_COMMNAD_CLEAR:
        {
            glClearColor( cmd->clearColor.r,
                          cmd->clearColor.g,
                          cmd->clearColor.b,
                          cmd->clearColor.a );
            glClearDepthf( cmd->clearDepth );
            glClearStencil( cmd->clearStencil );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        }
            break;

        case ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE:
        {
            switch( cmd->vertexFormat )
            {
                case ZP_VERTEX_FORMAT_VERTEX_COLOR:
                    glUseProgram( g_shaderVC.programShader.index );
                    break;

                case ZP_VERTEX_FORMAT_VERTEX_COLOR_UV:
                    glUseProgram( g_shaderVCU.programShader.index );
                    break;
            }

            BindVertexFormatForRenderCommand( cmd );

            static zp_float rot = 0;
            rot += 0.5f;
            if( rot > 360.f )
            {
                rot -= 360.f;
            }

            

            //glPushMatrix();
            //glRotatef( rot, 0, 1, 0 );
            GLenum mode = _TopologyToMode( cmd->topology );
            glDrawElementsBaseVertex( mode, static_cast<GLsizei>( cmd->indexCount ), GL_UNSIGNED_SHORT, reinterpret_cast<void*>( 0 ), static_cast<GLint>( cmd->indexOffset / sizeof( zp_ushort ) ) );
            //glPopMatrix();

            glUseProgram( 0 );

            UnbindVertexFormatForRenderCommand( cmd );
        }
            break;

        case ZP_RENDERING_COMMNAD_DRAW_BUFFERED:
            break;

        case ZP_RENDERING_COMMNAD_DRAW_INSTANCED:
            break;

        default:
            break;
    }
}

void PresentOpenGL( zp_handle hDC, zp_handle hContext )
{
    HDC dc = static_cast<HDC>( hDC );
    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( dc, context );

    //glViewport( 0, 0, 960, 640 );
    //glClearColor( 0.2058f, 0.3066f, 0.4877f, 1.0f );
    //glClearDepthf( 1.0f );
    //glClearStencil( 0 );
    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    //
    //glLoadIdentity();
    //
    //glBegin( GL_TRIANGLES );                      // Drawing Using Triangles
    //glColor4f( 1, 0, 0, 1 );
    //glVertex3f( 0.0f, 1.0f, 0.0f );              // Top
    //glColor4f( 0, 1, 0, 1 );
    //glVertex3f( -1.0f, -1.0f, 0.0f );              // Bottom Left
    //glColor4f( 0, 0, 1, 1 );
    //glVertex3f( 1.0f, -1.0f, 0.0f );              // Bottom Right
    //glEnd();                            // Finished Drawing The Triangle
    
    glFlush();

    SwapBuffers( dc );
}

void CreateRenderBufferOpenGL( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer )
{
    buffer.size = size;
    buffer.stride = 0;
    buffer.bufferType = type;
    buffer.bindType = bindType;

    GLenum target = _BufferTypeToTarget( type );
    GLenum usage = _BufferBindTypeToUsage( bindType );

    glGenBuffers( 1, &buffer.bufferIndex );
    glBindBuffer( target, buffer.bufferIndex );
    glBufferData( target, size, data, usage );
    glBindBuffer( target, 0 );
}

void DestroyRenderBufferOpenGL( zpRenderBuffer& buffer )
{
    glDeleteBuffers( 1, &buffer.bufferIndex );
    buffer.bufferIndex = 0;
}

void SetRenderBufferDataOpenGL( const zpRenderBuffer& buffer, const void* data, zp_size_t offset, zp_size_t length )
{
    GLenum target = _BufferTypeToTarget( buffer.bufferType );

    glBindBuffer( target, buffer.bufferIndex );
    glBufferSubData( target, offset, length, data );
    glBindBuffer( target, 0 );
}

void CreateTextureOpenGL( zp_uint width, zp_uint height, zp_int mipMapCount, zpDisplayFormat displayFormat, zpTextureDimension textureDimension, zpTextureType textureType, const void* pixels, zpTexture& texture )
{
    GLenum target = _TextureDimensionToTarget( textureDimension );
    GLint internalFormat = _DisplayFormatToInternalFormat( displayFormat );
    GLenum format = _DisplayFormatToFormat( displayFormat );
    GLenum type = _DisplayFormatToDataType( displayFormat );

    texture.width = width;
    texture.height = height;
    texture.textureDimension = textureDimension;
    texture.type = textureType;
    texture.format = displayFormat;

    glGenTextures( 1, &texture.textureIndex );
    glBindTexture( target, texture.textureIndex );
    glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

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
                glCompressedTexImage3D( target, 0, internalFormat, width, height, 0, 0, imageSize, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_CUBE_MAP:
                break;
            default:
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
                glTexImage3D( target, 0, internalFormat, width, height, 0, 0, format, type, pixels );
                break;
            case ZP_TEXTURE_DIMENSION_CUBE_MAP:
                break;
            default:
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
    glDeleteTextures( 1, &texture.textureIndex );
    texture.textureIndex = 0;
}

void CreateShaderOpenGL( const zp_char* vertexShaderSource, const zp_char* fragmentShaderSource, zpShader& shader )
{
    shader = {};

    GLint result;
    GLint infoLength;

    // vertex shader
    shader.vertexShader.index = glCreateShader( GL_VERTEX_SHADER );

    glShaderSource( shader.vertexShader.index, 1, &vertexShaderSource, ZP_NULL );
    glCompileShader( shader.vertexShader.index );

    glGetShaderiv( shader.vertexShader.index, GL_COMPILE_STATUS, &result );
    glGetShaderiv( shader.vertexShader.index, GL_INFO_LOG_LENGTH, &infoLength );
    if( infoLength )
    {
        zp_char buffer[ 512 ];
        glGetShaderInfoLog( shader.vertexShader.index, 512, ZP_NULL, buffer );
        zp_printfln( "Vertex Shader Error: %s", buffer );
    }

    // fragment shader
    shader.fragmentShader.index = glCreateShader( GL_FRAGMENT_SHADER );

    glShaderSource( shader.fragmentShader.index, 1, &fragmentShaderSource, ZP_NULL );
    glCompileShader( shader.fragmentShader.index );

    glGetShaderiv( shader.fragmentShader.index, GL_COMPILE_STATUS, &result );
    glGetShaderiv( shader.fragmentShader.index, GL_INFO_LOG_LENGTH, &infoLength );
    if( infoLength )
    {
        zp_char buffer[ 512 ];
        glGetShaderInfoLog( shader.fragmentShader.index, 512, ZP_NULL, buffer );
        zp_printfln( "Fragment Shader Error: %s", buffer );
    }

    // program
    shader.programShader.index = glCreateProgram();
    glAttachShader( shader.programShader.index, shader.vertexShader.index );
    glAttachShader( shader.programShader.index, shader.fragmentShader.index );
    glLinkProgram( shader.programShader.index );

    glGetProgramiv( shader.programShader.index, GL_LINK_STATUS, &result );
    glGetProgramiv( shader.programShader.index, GL_INFO_LOG_LENGTH, &infoLength );
    if( infoLength )
    {
        zp_char buffer[ 512 ];
        glGetProgramInfoLog( shader.programShader.index, 512, ZP_NULL, buffer );
        zp_printfln( "Program Link Error: %s", buffer );
    }

    glDetachShader( shader.programShader.index, shader.vertexShader.index );
    glDetachShader( shader.programShader.index, shader.fragmentShader.index );

    glDeleteShader( shader.vertexShader.index );
    glDeleteShader( shader.fragmentShader.index );

    shader.vertexShader.index = 0;
    shader.fragmentShader.index = 0;
}

void DestroyShaderOpenGL( zpShader& shader )
{
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

    if( shader.programShader.index )
    {
        glDeleteProgram( shader.programShader.index );
        shader.programShader.index = 0;
    }
}
