#include "Rendering/zpRendering.h"
#include <new>

const zp_hash64 ZP_TEXTURE_ID_INVALID = static_cast<zp_hash64>( -1 );
const zp_hash64 ZP_TEXTURE_ID_EMPTY = 0;
const zp_time_t ZP_TEXTURE_NOT_FILE = static_cast<zp_time_t>( -1 );

struct zpTextureData
{
    zpTextureDesc desc;
    zp_time_t lastModifiedTime;

    zpDataBuffer data;
};

zp_int LoadBMPTextureData( const zp_char* filepath, zpTextureData* textureData )
{
    zp_int result = 0;
    zp_byte header[ 54 ];
    zp_uint dataPos;
    zp_int width, height;
    zp_int imageSize;
    const zp_size_t bytesPerPixel = 3;

    zpFile bmpFile( filepath, ZP_FILE_MODE_BINARY_READ );
    if( bmpFile.getResult() != ZP_FILE_SUCCESS )
    {
        return 1;
    }

    textureData->lastModifiedTime = zpFile::lastModifiedTime( filepath );

    bmpFile.read( header, 0, sizeof( header ) );

    if( header[ 0 ] != 'B' && header[ 1 ] != 'M' )
    {
        return 1;
    }

    dataPos = *reinterpret_cast<zp_uint*>( header + 0x0A );
    width = *reinterpret_cast<zp_int*>( header + 0x12 );
    height = *reinterpret_cast<zp_int*>( header + 0x16 );
    imageSize = *reinterpret_cast<zp_int*>( header + 0x22 );

    if( imageSize == 0 )
    {
        imageSize = width * height * bytesPerPixel;
    }

    if( dataPos == 0 )
    {
        dataPos = 54;
    }

    zpTextureDesc desc = {};
    desc.width = width;
    desc.height = height;
    desc.format = ZP_DISPLAY_FORMAT_SRGB8;
    desc.textureDimension = ZP_TEXTURE_DIMENSION_2D;
    desc.type = ZP_TEXTURE_TYPE_TEXTURE;
    desc.mipMapCount = 0;

    textureData->desc = desc;
    textureData->data.reserve( imageSize );

    zp_size_t len;
    zp_byte buff[ 8192 ];
    while( ( len = bmpFile.read( buff, 0, sizeof( buff ) ) ) != 0 )
    {
        textureData->data.write( buff, 0, len );
    }

    zp_byte* d = textureData->data.getBuffer();

    // swap BGR to RGB
    zp_size_t imgSize = imageSize;
    for( zp_size_t i = 0; i < imgSize; i += bytesPerPixel )
    {
        d[ i ] ^= d[ i + 2 ] ^= d[ i ] ^= d[ i + 2 ];
    }

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
        zp_ushort colorMapOrigin;
        zp_ushort colorMapLength;
        zp_byte colorMapDepth;
        zp_ushort xOrigin;
        zp_ushort yOrigin;
        zp_ushort width;
        zp_ushort height;
        zp_byte bitsPerPixel;
        zp_byte imageDescriptor;
    };
    tgaHeader header;

    zpFile tgaFile( filepath, ZP_FILE_MODE_BINARY_READ );
    if( tgaFile.getResult() != ZP_FILE_SUCCESS )
    {
        return 1;
    }

    tgaFile.read( &header.idLenght, 0, sizeof( zp_byte ) );
    tgaFile.read( &header.colorMapType, 0, sizeof( zp_byte ) );
    tgaFile.read( &header.dataTypeCode, 0, sizeof( zp_byte ) );
    tgaFile.read( &header.colorMapOrigin, 0, sizeof( zp_ushort ) );
    tgaFile.read( &header.colorMapLength, 0, sizeof( zp_ushort ) );
    tgaFile.read( &header.colorMapDepth, 0, sizeof( zp_byte ) );
    tgaFile.read( &header.xOrigin, 0, sizeof( zp_ushort ) );
    tgaFile.read( &header.yOrigin, 0, sizeof( zp_ushort ) );
    tgaFile.read( &header.width, 0, sizeof( zp_ushort ) );
    tgaFile.read( &header.height, 0, sizeof( zp_ushort ) );
    tgaFile.read( &header.bitsPerPixel, 0, sizeof( zp_byte ) );
    tgaFile.read( &header.imageDescriptor, 0, sizeof( zp_byte ) );

    zpTextureDesc desc = {};
    desc.width = header.width;
    desc.height = header.height;
    desc.textureDimension = ZP_TEXTURE_DIMENSION_2D;
    desc.type = ZP_TEXTURE_TYPE_TEXTURE;
    desc.mipMapCount = 0;

    switch( header.bitsPerPixel )
    {
        case 24:
            desc.format = ZP_DISPLAY_FORMAT_SRGB8;
            break;

        case 32:
            desc.format = ZP_DISPLAY_FORMAT_SRGBA8;
            break;

        default:
            return 1;
    }

    textureData.desc = desc;

    textureData.lastModifiedTime = zpFile::lastModifiedTime( filepath );

    const zp_size_t pixelCount = header.width * header.height;
    const zp_size_t bytesPerPixel = header.bitsPerPixel / 8;
    const zp_size_t imageSize = pixelCount * bytesPerPixel;
    textureData.data.reserve( imageSize );

    switch( header.dataTypeCode )
    {
        // uncompressed rgb
        case 2:
        {
            zp_size_t len;
            zp_byte buff[ 8192 ];
            while( ( len = tgaFile.read( buff, 0, sizeof( buff ) ) ) != 0 )
            {
                textureData.data.write( buff, 0, len );
            }
        } break;

        // compressed RLE rgb
        case 10:
        {
            zp_byte buff[ 4 ];
            zp_byte chunckHeader;
            zp_size_t currentPixel = 0;

            do
            {
                tgaFile.read( &chunckHeader, 0, sizeof( zp_byte ) );

                if( chunckHeader < 128 )
                {
                    ++chunckHeader;

                    for( zp_size_t c = 0; c < chunckHeader; ++c )
                    {
                        tgaFile.read( buff, 0, bytesPerPixel );
                        textureData.data.write( buff, 0, bytesPerPixel );
                    }
                }
                else
                {
                    chunckHeader -= 127;

                    tgaFile.read( buff, 0, bytesPerPixel );

                    for( zp_size_t c = 0; c < chunckHeader; ++c )
                    {
                        textureData.data.write( buff, 0, bytesPerPixel );
                    }
                }

                currentPixel += chunckHeader;
            } while( currentPixel < pixelCount );
        } break;

        default:
            ZP_INVALID_CODE_PATH();
            break;
    }

    zp_byte* d = textureData.data.getBuffer();

    // swap BGR to RGB
    for( zp_size_t i = 0; i < imageSize; i += bytesPerPixel )
    {
        d[ i ] ^= d[ i + 2 ] ^= d[ i ] ^= d[ i + 2 ];
    }

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
    : m_textureInstances( 64 )
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
    ZP_ASSERT( m_textureInstances.isEmpty(), "Texture(s) still loaded: %d", m_textureInstances.size() );

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
            m_engine->destroyTexture( t->texture );
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
        foundTextureInstance = new( g_globalAllocator->allocate( sizeof( zpTextureInstance ) ) ) zpTextureInstance;
        m_textureInstances.pushBack( foundTextureInstance );
    }

    foundTextureInstance->lastModifiedTime = ZP_TEXTURE_NOT_FILE;
    foundTextureInstance->refCount = 0;
    foundTextureInstance->instanceId = ++m_newTextureInstanceId;
    foundTextureInstance->textureName = textureName;

    texture.set( foundTextureInstance->instanceId, foundTextureInstance );

    return true;
}

zp_bool zpTextureManager::loadTexture( const zp_char* textureFile, zpTextureHandle& texture )
{
    zpTextureInstance* foundTextureInstance = ZP_NULL;
    zpTextureInstance** b = m_textureInstances.begin();
    zpTextureInstance** e = m_textureInstances.end();
    for( ; b != e; ++b )
    {
        zpTextureInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            m_engine->destroyTexture( t->texture );
            foundTextureInstance = t;
            break;
        }
        else if( zp_strcmp( textureFile, t->textureName.str() ) == 0 )
        {
            texture.set( t->instanceId, t );
            return true;
        }
    }

    zp_int r = 0;
    zpTextureData textureData = {};

    zp_size_t len = zp_strlen( textureFile );
    if( zp_stricmp( textureFile + ( len - 4 ), ".tga" ) == 0 )
    {
        r = LoadTGATextureData( textureFile, textureData );
    }
    else if( zp_stricmp( textureFile + ( len - 4 ), ".bmp" ) == 0 )
    {
        r = LoadBMPTextureData( textureFile, &textureData );
    }
    else
    {
        ZP_ASSERT( false, "" );
    }

    if( r )
    {
        return false;
    }

    if( foundTextureInstance == ZP_NULL )
    {
        foundTextureInstance = new( g_globalAllocator->allocate( sizeof( zpTextureInstance ) ) ) zpTextureInstance;
        m_textureInstances.pushBack( foundTextureInstance );
    }

    zpCreateTextureDesc desc = {};
    desc.textureName = textureFile;
    desc.pixels = textureData.data.getBuffer();
    desc.desc = textureData.desc;

    m_engine->createTexture( &desc, foundTextureInstance->texture );

    foundTextureInstance->lastModifiedTime = textureData.lastModifiedTime;
    foundTextureInstance->refCount = 0;
    foundTextureInstance->instanceId = ++m_newTextureInstanceId;
    foundTextureInstance->textureName = textureFile;

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
            m_engine->destroyTexture( b->texture );
            b->~zpTextureInstance();

            g_globalAllocator->free( b );

            m_textureInstances.eraseAtSwapBack( i );

            --i;
            --imax;
        }
    }
}

void zpTextureManager::reloadChangedTextures()
{
    zpTextureInstance** b = m_textureInstances.begin();
    zpTextureInstance** e = m_textureInstances.end();
    for( ; b != e; ++b )
    {
        zpTextureInstance* t = *b;
        if( t->refCount > 0 && t->lastModifiedTime != ZP_TEXTURE_NOT_FILE )
        {
            const zp_char* textureName = t->textureName.str();
            zp_time_t lastModTime = zpFile::lastModifiedTime( textureName );
            if( t->lastModifiedTime != lastModTime )
            {
                zp_int r = 0;
                zpTextureData textureData = {};
                zp_size_t len = zp_strlen( textureName );
                if( zp_stricmp( textureName + ( len - 4 ), ".tga" ) == 0 )
                {
                    r = LoadTGATextureData( textureName, textureData );
                }
                else if( zp_stricmp( textureName + ( len - 4 ), ".bmp" ) == 0 )
                {
                    r = LoadBMPTextureData( textureName, &textureData );
                }
                else
                {
                    ZP_ASSERT( false, "" );
                }
                
                if( r )
                {
                    continue;
                }

                t->lastModifiedTime = lastModTime;

                zpCreateTextureDesc desc = {};
                desc.textureName = textureName;
                desc.pixels = textureData.data.getBuffer();
                desc.desc = textureData.desc;

                zpTexture tex = t->texture;
                m_engine->createTexture( &desc, t->texture );

                m_engine->destroyTexture( tex );
            }
        }
    }
}
