#include "Rendering/zpRendering.h"
#include <new>

const zp_hash64 ZP_FONT_ID_INVALID = static_cast<zp_hash64>( -1 );
const zp_hash64 ZP_FONT_ID_EMPTY = 0;
const zp_time_t ZP_FONT_NOT_FILE = static_cast<zp_time_t>( -1 );

struct zpFontData
{
    zp_time_t lastModifiedTime;
    zpString materialFile;


};

zp_int LoadFontData( const zp_char* fontFile, zpFontData& fontData )
{
    zp_int r = 0;
    return r;
}

//
//
//

zpFontHandle::zpFontHandle()
    : m_instanceId( ZP_FONT_ID_INVALID )
    , m_fontInstance( ZP_NULL )
{
}

zpFontHandle::zpFontHandle( const zpFontHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_fontInstance( other.m_fontInstance )
{
    addRef();
}

zpFontHandle::zpFontHandle( zpFontHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_fontInstance( other.m_fontInstance )
{
    addRef();
    other.release();
}

zpFontHandle::~zpFontHandle()
{
    release();
}

zpFontHandle& zpFontHandle::operator=( const zpFontHandle& other )
{
    set( other.m_instanceId, other.m_fontInstance );

    return *this;
}
zpFontHandle& zpFontHandle::operator=( zpFontHandle&& other )
{
    set( other.m_instanceId, other.m_fontInstance );

    other.release();

    return *this;
}

const zpFont* zpFontHandle::get() const
{
    return isValid() ? &m_fontInstance->font : ZP_NULL;
}
zpFont* zpFontHandle::get()
{
    return isValid() ? &m_fontInstance->font : ZP_NULL;
}

zp_bool zpFontHandle::isValid() const
{
    return m_fontInstance != ZP_NULL && m_fontInstance->instanceId == m_instanceId;
}

void zpFontHandle::release()
{
    releaseRef();

    m_instanceId = ZP_FONT_ID_INVALID;
    m_fontInstance = ZP_NULL;
}

void zpFontHandle::addRef()
{
    if( isValid() )
    {
        ++m_fontInstance->refCount;
    }
}

void zpFontHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_fontInstance->refCount != 0, "" );
        --m_fontInstance->refCount;
    }
}

void zpFontHandle::set( zp_hash64 instanceId, zpFontInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_fontInstance = objectInstance;

    addRef();
}

//
//
//

zpFontManager::zpFontManager()
    : m_fontInstances( 64 )
    , m_materialManager( ZP_NULL )
    , m_newFontInstanceId( ZP_FONT_ID_EMPTY )
{
}
zpFontManager::~zpFontManager()
{
}

void zpFontManager::setup( zpMaterialManager* materialManager )
{
    m_materialManager = materialManager;
}

void zpFontManager::teardown()
{
    m_materialManager = ZP_NULL;
}

zp_bool zpFontManager::getFont( const zp_char* fontName, zpFontHandle& font )
{
    zpFontInstance* foundFontInstance = ZP_NULL;
    zpFontInstance** b = m_fontInstances.begin();
    zpFontInstance** e = m_fontInstances.end();
    for( ; b != e; ++b )
    {
        zpFontInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            t->font.fontMaterial.release();
            foundFontInstance = t;
            break;
        }
        else if( zp_strcmp( fontName, t->fontName.str() ) == 0 )
        {
            font.set( t->instanceId, t );
            return true;
        }
    }

    if( foundFontInstance == ZP_NULL )
    {
        foundFontInstance = new( g_globalAllocator.allocate( sizeof( zpFontInstance ) ) ) zpFontInstance;
        m_fontInstances.pushBack( foundFontInstance );
    }

    foundFontInstance->lastModifiedTime = ZP_FONT_NOT_FILE;
    foundFontInstance->refCount = 0;
    foundFontInstance->instanceId = ++m_newFontInstanceId;
    foundFontInstance->fontName = fontName;

    font.set( foundFontInstance->instanceId, foundFontInstance );

    return true;
}

zp_bool zpFontManager::loadFont( const zp_char* fontFile, zpFontHandle& font )
{
    zpFontInstance* foundFontInstance = ZP_NULL;
    zpFontInstance** b = m_fontInstances.begin();
    zpFontInstance** e = m_fontInstances.end();
    for( ; b != e; ++b )
    {
        zpFontInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            t->font.fontMaterial.release();
            foundFontInstance = t;
            break;
        }
        else if( zp_strcmp( fontFile, t->fontName.str() ) == 0 )
        {
            font.set( t->instanceId, t );
            return true;
        }
    }

    zp_int r = 0;
    zpFontData fontData;

    r = LoadFontData( fontFile, fontData );
    if( r )
    {
        return false;
    }

    if( foundFontInstance == ZP_NULL )
    {
        foundFontInstance = new( g_globalAllocator.allocate( sizeof( zpFontInstance ) ) ) zpFontInstance;
        m_fontInstances.pushBack( foundFontInstance );
    }

    m_materialManager->loadMaterial( fontData.materialFile.str(), foundFontInstance->font.fontMaterial );
    
    foundFontInstance->lastModifiedTime = fontData.lastModifiedTime;
    foundFontInstance->refCount = 0;
    foundFontInstance->instanceId = ++m_newFontInstanceId;
    foundFontInstance->fontName = fontFile;

    font.set( foundFontInstance->instanceId, foundFontInstance );

    return true;
}

void zpFontManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_fontInstances.size(); i != imax; ++i )
    {
        zpFontInstance* b = m_fontInstances[ i ];
        if( b->refCount == 0 )
        {
            b->font.fontMaterial.release();
            b->~zpFontInstance();

            g_globalAllocator.free( b );

            m_fontInstances.erase( i );

            --i;
            --imax;
        }
    }
}

void zpFontManager::reloadChangedFonts()
{
    zpFontInstance** b = m_fontInstances.begin();
    zpFontInstance** e = m_fontInstances.end();
    for( ; b != e; ++b )
    {
        zpFontInstance* t = *b;
        if( t->refCount > 0 && t->lastModifiedTime != ZP_FONT_NOT_FILE )
        {
            const zp_char* fontName = t->fontName.str();
            zp_time_t lastModTime = zpFile::lastModifiedTime( fontName );
            if( t->lastModifiedTime != lastModTime )
            {
                zp_int r = 0;
                zpFontData fontData;
               
                r = LoadFontData( fontName, fontData );

                if( r )
                {
                    continue;
                }

                t->lastModifiedTime = lastModTime;

                t->font.fontMaterial.release();
            }
        }
    }
}
