#include "Rendering/zpRendering.h"
//#include <new>

const zp_hash64 ZP_SHADER_ID_INVALID = (zp_hash64)-1;
const zp_hash64 ZP_SHADER_ID_EMPTY = 0;

zp_bool operator==( const zpShaderProperty& x, const zpShaderProperty& y )
{
    return x.m_property == y.m_property;
}

zp_bool operator!=( const zpShaderProperty& x, const zpShaderProperty& y )
{
    return x.m_property != y.m_property;
}

//
//
//

zp_bool operator==( const zpShaderKeyword& x, const zpShaderKeyword& y )
{
    return x.m_keyword == y.m_keyword;
}

zp_bool operator!=( const zpShaderKeyword& x, const zpShaderKeyword& y )
{
    return x.m_keyword != y.m_keyword;
}

zp_bool operator<( const zpShaderKeyword& x, const zpShaderKeyword& y )
{
    return x.m_keyword < y.m_keyword;
}

//
//
//

struct zpShaderData
{

};

zp_int LoadShaderData( const zp_char* filepath, zpShaderData& shaderData )
{
    zpFile shaderFile( filepath, ZP_FILE_MODE_BINARY_READ );
    if( shaderFile.getResult() != ZP_FILE_SUCCESS )
    {
        return -1;
    }



    return 0;
}

//
//
//

zpShaderHandle::zpShaderHandle()
    : m_instanceId( ZP_SHADER_ID_INVALID )
    , m_shaderInstance( ZP_NULL )
{
}

zpShaderHandle::zpShaderHandle( const zpShaderHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_shaderInstance( other.m_shaderInstance )
{
    addRef();
}

zpShaderHandle::zpShaderHandle( zpShaderHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_shaderInstance( other.m_shaderInstance )
{
    addRef();
    other.release();
}

zpShaderHandle::~zpShaderHandle()
{
    release();
}

zpShaderHandle& zpShaderHandle::operator=( const zpShaderHandle& other )
{
    set( other.m_instanceId, other.m_shaderInstance );

    return *this;
}
zpShaderHandle& zpShaderHandle::operator=( zpShaderHandle&& other )
{
    set( other.m_instanceId, other.m_shaderInstance );

    other.release();

    return *this;
}

const zpShader* zpShaderHandle::operator->() const
{
    return isValid() ? &m_shaderInstance->shader : ZP_NULL;
}
zpShader* zpShaderHandle::operator->()
{
    return isValid() ? &m_shaderInstance->shader : ZP_NULL;
}

zp_bool zpShaderHandle::isValid() const
{
    return m_shaderInstance != ZP_NULL && m_shaderInstance->instanceId == m_instanceId;
}

void zpShaderHandle::release()
{
    releaseRef();

    m_instanceId = ZP_SHADER_ID_INVALID;
    m_shaderInstance = ZP_NULL;
}

void zpShaderHandle::addRef()
{
    if( isValid() )
    {
        ++m_shaderInstance->refCount;
    }
}

void zpShaderHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_shaderInstance->refCount != 0, "" );
        --m_shaderInstance->refCount;
    }
}

void zpShaderHandle::set( zp_hash64 instanceId, zpShaderInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_shaderInstance = objectInstance;

    addRef();
}

//
//
//

zpShaderManager::zpShaderManager()
    : m_shaderInstances( 64 )
    , m_engine( ZP_NULL )
    , m_newShaderInstanceId( ZP_SHADER_ID_EMPTY )
{
}
zpShaderManager::~zpShaderManager()
{
}

void zpShaderManager::setup( zpRenderingEngine* engine )
{
    m_engine = engine;
    zpShaderProperty::s_shaderPropertyToName = &m_shaderPropertyToName;
    zpShaderKeyword::s_shaderKeywordToName = &m_shaderKeywordToName;
}

void zpShaderManager::teardown()
{
    zpShaderKeyword::s_shaderKeywordToName = ZP_NULL;
    zpShaderProperty::s_shaderPropertyToName = ZP_NULL;

    m_shaderPropertyToName.clear();
    m_shaderKeywordToName.clear();

    m_engine = ZP_NULL;
}

zp_bool zpShaderManager::getShader( const zp_char* shaderName, zpShaderHandle& shader )
{
    zpShaderInstance* foundShaderInstance = ZP_NULL;
    zpShaderInstance** b = m_shaderInstances.begin();
    zpShaderInstance** e = m_shaderInstances.end();
    for( ; b != e; ++b )
    {
        zpShaderInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            m_engine->destroyShader( t->shader );
            foundShaderInstance = t;
            break;
        }
        else if( zp_strcmp( shaderName, t->shaderName.str() ) == 0 )
        {
            shader.set( t->instanceId, t );
            return true;
        }
    }

    if( foundShaderInstance == ZP_NULL )
    {
        foundShaderInstance = new( g_globalAllocator->allocate( sizeof( zpShaderInstance ) ) ) zpShaderInstance;
        m_shaderInstances.pushBack( foundShaderInstance );
    }

    zpShaderData shaderData;
    LoadShaderData( shaderName, shaderData );

    zpCreateShaderDesc shaderDesc = {};
    shaderDesc.shaderName = shaderName;

    m_engine->createShader( &shaderDesc, foundShaderInstance->shader );

    foundShaderInstance->refCount = 0;
    foundShaderInstance->instanceId = ++m_newShaderInstanceId;
    foundShaderInstance->shaderName = shaderName;

    shader.set( foundShaderInstance->instanceId, foundShaderInstance );

    return true;
}

void zpShaderManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_shaderInstances.size(); i != imax; ++i )
    {
        zpShaderInstance* b = m_shaderInstances[ i ];
        if( b->refCount == 0 )
        {
            m_engine->destroyShader( b->shader );
            b->~zpShaderInstance();

            g_globalAllocator->free( b );

            m_shaderInstances.eraseAtSwapBack( i );

            --i;
            --imax;
        }
    }
}

zpShaderProperty zpShaderManager::getShaderProperty( const zp_char* name ) const
{
    zpShaderProperty property;

    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    if( m_shaderPropertyToName.containsKey( hash ) )
    {
        property = zpShaderProperty( hash );
    }

    return property;
}

zpShaderKeyword zpShaderManager::getShaderKeyword( const zp_char* name ) const
{
    zpShaderKeyword keyword;

    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    if( m_shaderKeywordToName.containsKey( hash ) )
    {
        keyword = zpShaderKeyword( hash );
    }

    return keyword;
}

//
//
//

zpMap<zp_hash_t, zpString>* zpShaderProperty::s_shaderPropertyToName = ZP_NULL; 
zpMap<zp_hash_t, zpString>* zpShaderKeyword::s_shaderKeywordToName = ZP_NULL;