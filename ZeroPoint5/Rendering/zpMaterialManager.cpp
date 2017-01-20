#include "zpRendering.h"
#include <new>


const zp_hash64 ZP_MATERIAL_ID_INVALID = (zp_hash64)-1;
const zp_hash64 ZP_MATERIAL_ID_EMPTY = 0;

struct zpMaterialData
{
    zp_time_t lastModifiedTime;

    zpColorf color;

    zpVector4fData mainTexST;
    zpVector4fData specTexST;
    zpVector4fData normTexST;

    zpString shaderName;
    zpString mainTexName;
    zpString specTexName;
    zpString normTexName;
};

zp_int LoadMaterialData( const zp_char* filepath, zpMaterialData& materialData )
{
    //zpFile materialFile( filepath, ZP_FILE_MODE_BINARY_READ );
    //if( materialFile.getResult() != ZP_FILE_SUCCESS )
    //{
    //    return -1;
    //}

    materialData.color = { 1, 1, 1, 1 };

    materialData.mainTexST = { 1, 1, 0, 0 };
    materialData.specTexST = { 1, 1, 0, 0 };
    materialData.normTexST = { 1, 1, 0, 0 };

    materialData.mainTexName = "Assets/uv_checker_large.bmp";

    return 0;
}

//
//
//

zpMaterialHandle::zpMaterialHandle()
    : m_instanceId( ZP_MATERIAL_ID_INVALID )
    , m_materialInstance( ZP_NULL )
{
}

zpMaterialHandle::zpMaterialHandle( const zpMaterialHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_materialInstance( other.m_materialInstance )
{
    addRef();
}

zpMaterialHandle::zpMaterialHandle( zpMaterialHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_materialInstance( other.m_materialInstance )
{
    addRef();
    other.release();
}

zpMaterialHandle::~zpMaterialHandle()
{
    release();
}

zpMaterialHandle& zpMaterialHandle::operator=( const zpMaterialHandle& other )
{
    set( other.m_instanceId, other.m_materialInstance );

    return *this;
}
zpMaterialHandle& zpMaterialHandle::operator=( zpMaterialHandle&& other )
{
    set( other.m_instanceId, other.m_materialInstance );

    other.release();

    return *this;
}

const zpMaterial* zpMaterialHandle::operator->() const
{
    return isValid() ? &m_materialInstance->material : ZP_NULL;
}
zpMaterial* zpMaterialHandle::operator->()
{
    return isValid() ? &m_materialInstance->material : ZP_NULL;
}

zp_bool zpMaterialHandle::isValid() const
{
    return m_materialInstance != ZP_NULL && m_materialInstance->instanceId == m_instanceId;
}

void zpMaterialHandle::release()
{
    releaseRef();

    m_instanceId = ZP_MATERIAL_ID_INVALID;
    m_materialInstance = ZP_NULL;
}

void zpMaterialHandle::addRef()
{
    if( isValid() )
    {
        ++m_materialInstance->refCount;
    }
}

void zpMaterialHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_materialInstance->refCount != 0, "" );
        --m_materialInstance->refCount;
    }
}

void zpMaterialHandle::set( zp_hash64 instanceId, zpMaterialInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_materialInstance = objectInstance;

    addRef();
}

//
//
//

zpMaterialManager::zpMaterialManager()
    : m_materialInstances( 64 )
    , m_shaderManager( ZP_NULL )
    , m_textureManager( ZP_NULL )
    , m_newMaterialInstanceId( ZP_MATERIAL_ID_EMPTY )
{
}
zpMaterialManager::~zpMaterialManager()
{
}

void zpMaterialManager::setup( zpShaderManager* shaderManager, zpTextureManager* textureManager )
{
    m_shaderManager = shaderManager;
    m_textureManager = textureManager;
}

void zpMaterialManager::teardown()
{
    m_shaderManager = ZP_NULL;
    m_textureManager = ZP_NULL;
}

zp_bool zpMaterialManager::getMaterial( const zp_char* materialName, zpMaterialHandle& material )
{
    zpMaterialInstance* foundMaterialInstance = ZP_NULL;
    zpMaterialInstance** b = m_materialInstances.begin();
    zpMaterialInstance** e = m_materialInstances.end();
    for( ; b != e; ++b )
    {
        zpMaterialInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            t->material.shader.release();
            t->material.mainTex.release();
            t->material.normTex.release();
            t->material.specTex.release();

            foundMaterialInstance = t;
            break;
        }
        else if( zp_strcmp( materialName, t->materialName.str() ) == 0 )
        {
            material.set( t->instanceId, t );
            return true;
        }
    }

    if( foundMaterialInstance == ZP_NULL )
    {
        foundMaterialInstance = new( g_globalAllocator.allocate( sizeof( zpMaterialInstance ) ) ) zpMaterialInstance;
        m_materialInstances.pushBack( foundMaterialInstance );
    }

    zpMaterialData materialData;
    LoadMaterialData( materialName, materialData );

    foundMaterialInstance->material.color = materialData.color;

    foundMaterialInstance->material.mainTexST = materialData.mainTexST;
    foundMaterialInstance->material.specTexST = materialData.specTexST;
    foundMaterialInstance->material.normTexST = materialData.normTexST;

    if( !materialData.shaderName.isEmpty() ) m_shaderManager->getShader( materialData.shaderName.str(), foundMaterialInstance->material.shader );

    if( !materialData.mainTexName.isEmpty() ) m_textureManager->getTexture( materialData.mainTexName.str(), foundMaterialInstance->material.mainTex );
    if( !materialData.specTexName.isEmpty() ) m_textureManager->getTexture( materialData.specTexName.str(), foundMaterialInstance->material.specTex );
    if( !materialData.normTexName.isEmpty() ) m_textureManager->getTexture( materialData.normTexName.str(), foundMaterialInstance->material.normTex );

    foundMaterialInstance->lastModifiedTime = materialData.lastModifiedTime;
    foundMaterialInstance->refCount = 0;
    foundMaterialInstance->instanceId = ++m_newMaterialInstanceId;
    foundMaterialInstance->materialName = materialName;

    material.set( foundMaterialInstance->instanceId, foundMaterialInstance );

    return true;
}

void zpMaterialManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_materialInstances.size(); i != imax; ++i )
    {
        zpMaterialInstance* b = m_materialInstances[ i ];
        if( b->refCount == 0 )
        {
            b->~zpMaterialInstance();

            g_globalAllocator.free( b );

            m_materialInstances.erase( i );

            --i;
            --imax;
        }
    }
}
