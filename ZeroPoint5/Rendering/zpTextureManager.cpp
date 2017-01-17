#include "zpRendering.h"
#include <new>

const zp_hash64 ZP_TEXTURE_ID_INVALID = (zp_hash64)-1;
const zp_hash64 ZP_TEXTURE_ID_EMPTY = 0;

struct zpTextureData
{
    zp_uint width;
    zp_uint height;
    zpTextureDimension textureDimension;
    zpTextureType type;
    zpDisplayFormat format;

    zpDataBuffer data;
};

zp_int LoadBMPTextureData( const zp_char* filepath, zpTextureData& textureData )
{
    zp_int result = 0;
    zp_byte header[ 54 ];
    zp_uint dataPos;
    zp_int width, height;
    zp_int imageSize;
    
    zpFile bmpFile( filepath );
    bmpFile.open( ZP_FILE_MODE_BINARY_READ );
    bmpFile.read( header, 0, sizeof( header ) );

    if( header[ 0 ] != 'B' && header[ 1 ] != 'M' )
    {
        bmpFile.close();
        return -1;
    }

    dataPos = *reinterpret_cast<zp_uint*>( header + 0x0A );
    width = *reinterpret_cast<zp_int*>( header + 0x12 );
    height = *reinterpret_cast<zp_int*>( header + 0x16 );
    imageSize = *reinterpret_cast<zp_int*>( header + 0x22 );

    if( imageSize == 0 )
    {
        imageSize = width * height * 3;
    }

    if( dataPos == 0 )
    {
        dataPos = 54;
    }

    textureData.width = width;
    textureData.height = height;
    textureData.textureDimension = ZP_TEXTURE_DIMENSION_2D;
    textureData.type = ZP_TEXTURE_TYPE_TEXTURE;
    textureData.format = ZP_DISPLAY_FORMAT_RGB8_UINT;

    zp_size_t bytesPerPixel = 3;
    zp_size_t len;
    zp_byte buff[ 8192 ];
    textureData.data.reserve( imageSize );
    while( ( len = bmpFile.read( buff, 0, 8192 ) ) != 0 )
    {
        textureData.data.write( buff, 0, len );
    }

    zp_byte* d = textureData.data.getBuffer();

    // swap BGR to RGB
    for( zp_size_t i = 0; i < imageSize; i += bytesPerPixel )
    {
        d[ i ] ^= d[ i + 2 ] ^= d[ i ] ^= d[ i + 2 ];
    }

    bmpFile.close();
    return result;
}

zp_int LoadTGATextureData( const zp_char* filepath, zpTextureData& textureData )
{
    zp_int result = 0;

    struct tgaHeader
    {
        zp_byte idLenght;
        zp_byte colorMapType;
        zp_byte dataTypeCode;
        zp_short colorMapOrigin;
        zp_short colorMapLength;
        zp_byte colorMapDepth;
        zp_short xOrigin;
        zp_short yOrigin;
        zp_short width;
        zp_short height;
        zp_byte bitsPerPixel;
        zp_char imageDescriptor;
    };
    tgaHeader header;

    zpFile bmpFile( filepath );
    bmpFile.open( ZP_FILE_MODE_BINARY_READ );
    bmpFile.read( &header, 0, sizeof( tgaHeader ) );

    textureData.width = header.width;
    textureData.height = header.height;
    textureData.textureDimension = ZP_TEXTURE_DIMENSION_2D;
    textureData.type = ZP_TEXTURE_TYPE_TEXTURE;

    switch( header.bitsPerPixel )
    {
        case 24:
            textureData.format = ZP_DISPLAY_FORMAT_RGB8_UINT;
            break;

        case 32:
            textureData.format = ZP_DISPLAY_FORMAT_RGBA8_UINT;
            break;

        default:
            return -1;
    }

    zp_size_t bytesPerPixel = header.bitsPerPixel / 8;
    zp_size_t imageSize = header.width * header.height * bytesPerPixel;

    zp_size_t len;
    zp_byte buff[ 8192 ];
    textureData.data.reserve( imageSize );
    while( ( len = bmpFile.read( buff, 0, 8192 ) ) != 0 )
    {
        textureData.data.write( buff, 0, len );
    }

    zp_byte* d = textureData.data.getBuffer();

    // swap BGR to RGB
    for( zp_size_t i = 0; i < imageSize; i += bytesPerPixel )
    {
        d[ i ] ^= d[ i + 2 ] ^= d[ i ] ^= d[ i + 2 ];
    }

    bmpFile.close();

    return result;
}

//
//
//

zpTextureHandle::zpTextureHandle()
    : m_instanceId( ZP_TEXTURE_ID_INVALID )
    , m_textureInstance( ZP_NULL )
{
}

zpTextureHandle::zpTextureHandle( const zpTextureHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_textureInstance( other.m_textureInstance )
{
    addRef();
}

zpTextureHandle::zpTextureHandle( zpTextureHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_textureInstance( other.m_textureInstance )
{
    addRef();
    other.release();
}

zpTextureHandle::~zpTextureHandle()
{
    release();
}

zpTextureHandle& zpTextureHandle::operator=( const zpTextureHandle& other )
{
    set( other.m_instanceId, other.m_textureInstance );

    return *this;
}
zpTextureHandle& zpTextureHandle::operator=( zpTextureHandle&& other )
{
    set( other.m_instanceId, other.m_textureInstance );

    other.release();

    return *this;
}

const zpTexture* zpTextureHandle::operator->() const
{
    return isValid() ? &m_textureInstance->texture : ZP_NULL;
}
zpTexture* zpTextureHandle::operator->()
{
    return isValid() ? &m_textureInstance->texture : ZP_NULL;
}

zp_bool zpTextureHandle::isValid() const
{
    return m_textureInstance != ZP_NULL && m_textureInstance->instanceId == m_instanceId;
}

void zpTextureHandle::release()
{
    releaseRef();

    m_instanceId = ZP_TEXTURE_ID_INVALID;
    m_textureInstance = ZP_NULL;
}

void zpTextureHandle::addRef()
{
    if( isValid() )
    {
        ++m_textureInstance->refCount;
    }
}

void zpTextureHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_textureInstance->refCount != 0, "" );
        --m_textureInstance->refCount;
    }
}

void zpTextureHandle::set( zp_hash64 instanceId, zpTextureInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_textureInstance = objectInstance;

    addRef();
}

//
//
//

zpTextureManager::zpTextureManager()
    : m_textureMemory( sizeof( zpTextureInstance ) * 64 )
    , m_textureInstances( 64 )
    , m_engine( ZP_NULL )
    , m_newTextureInstanceId( ZP_TEXTURE_ID_EMPTY )
{
}
zpTextureManager::~zpTextureManager()
{
}

void zpTextureManager::setup( zpRenderingEngine* engine )
{
    m_engine = engine;
}

void zpTextureManager::teardown()
{
    m_engine = ZP_NULL;
}

zp_bool zpTextureManager::getTexture( const zp_char* textureName, zpTextureHandle& texture )
{
    zpTextureInstance* foundTextureInstance = ZP_NULL;
    zpTextureInstance** b = m_textureInstances.begin();
    zpTextureInstance** e = m_textureInstances.end();
    for( ; b != e; ++b )
    {
        zpTextureInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            m_engine->destoryTexture( t->texture );
            foundTextureInstance = t;
            break;
        }
        else if( zp_strcmp( textureName, t->textureName.str() ) == 0 )
        {
            texture.set( t->instanceId, t );
            return true;
        }
    }

    if( foundTextureInstance == ZP_NULL )
    {
        foundTextureInstance = new( m_textureMemory.allocate( sizeof( zpTextureInstance ) ) ) zpTextureInstance;
        m_textureInstances.pushBack( foundTextureInstance );
    }

    zpTextureData textureData;
    zp_size_t len = zp_strlen( textureName );
    if( zp_stricmp( textureName + ( len - 4 ), ".tga" ) == 0 )
    {
        LoadTGATextureData( textureName, textureData );
    }
    else if( zp_stricmp( textureName + ( len - 4 ), ".bmp" ) == 0 )
    {
        LoadBMPTextureData( textureName, textureData );
    }

    m_engine->createTexture( textureData.width,
                             textureData.height,
                             0, 
                             textureData.format, 
                             textureData.textureDimension, 
                             textureData.type, 
                             textureData.data.getBuffer(), 
                             foundTextureInstance->texture );

    foundTextureInstance->refCount = 0;
    foundTextureInstance->instanceId = ++m_newTextureInstanceId;
    foundTextureInstance->textureName = textureName;

    texture.set( foundTextureInstance->instanceId, foundTextureInstance );

    return true;
}

void zpTextureManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_textureInstances.size(); i != imax; ++i )
    {
        zpTextureInstance* b = m_textureInstances[ i ];
        if( b->refCount == 0 )
        {
            m_engine->destoryTexture( b->texture );
            b->~zpTextureInstance();

            m_textureMemory.free( b );

            m_textureInstances.erase( i );

            --i;
            --imax;
        }
    }
}
