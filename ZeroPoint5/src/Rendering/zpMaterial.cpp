#include "Rendering/zpRendering.h"
#include <new>

const zp_hash64 ZP_MATERIAL_ID_INVALID = static_cast<zp_hash64>( -1 );
const zp_hash64 ZP_MATERIAL_ID_EMPTY = 0;
const zp_time_t ZP_MATERIAL_NOT_FILE = static_cast<zp_time_t>( -1 );

struct zpMaterialDataPartColor
{
    zpString name;
    zpColorf color;
};

struct zpMaterialDataPartVector
{
    zpString name;
    zpVector4fData vector;
};

struct zpMaterialDataPartTexture
{
    zpString name;
    zpString texture;
};

struct zpMaterialDataPartMatrix
{
    zpString name;
    zpMatrix4fData matrix;
};


struct zpMaterialData
{
    zp_time_t lastModifiedTime;

    zpString shaderName;

    zpMap< zpShaderProperty, zp_int > ints;
    zpMap< zpShaderProperty, zp_float > floats;
    zpMap< zpShaderProperty, zpColorf > colors;
    zpMap< zpShaderProperty, zpVector4fData > vectors;
    zpMap< zpShaderProperty, zpString > textures;
    zpMap< zpShaderProperty, zpMatrix4fData > matrices;

};

static zp_int LoadMaterialData( const zp_char* filepath, zpMaterialData& materialData )
{
    //zpFile materialFile( filepath, ZP_FILE_MODE_BINARY_READ );
    //if( materialFile.getResult() != ZP_FILE_SUCCESS )
    //{
    //    return -1;
    //}

    materialData.colors.set( "_Color", zpColor::White );
    
    materialData.textures.set( "_MainTex", zpString( "Assets/uv_checker_large.bmp" ) );
    materialData.vectors.set( "_MainTex_ST", { 1, 1, 0, 0 } );

    return 0;
}

static void ClearMaterial( zpMaterial& material )
{
    material.ints.clear();
    material.floats.clear();
    material.colors.clear();
    material.vectors.clear();
    material.matrices.clear();
    material.textures.clear();
}

static void FillMaterial( zpShaderManager* shaderManager, zpTextureManager* textureManager, const zpMaterialData& materialData, zpMaterial& material )
{
    if( !materialData.shaderName.isEmpty() ) shaderManager->getShader( materialData.shaderName.str(), material.shader );

    ClearMaterial( material );

    material.ints.setAll( materialData.ints );
    material.floats.setAll( materialData.floats );
    material.colors.setAll( materialData.colors );
    material.vectors.setAll( materialData.vectors );
    material.matrices.setAll( materialData.matrices );

    zpMap< zpShaderProperty, zpString >::const_iterator tb = materialData.textures.begin();
    zpMap< zpShaderProperty, zpString >::const_iterator te = materialData.textures.end();
    for( ; tb != te; ++tb )
    {
        zpTextureHandle tex;
        textureManager->loadTexture( tb.value().str(), tex );
        material.textures.set( tb.key(), tex );
    }
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

const zpMaterial* zpMaterialHandle::get() const
{
    return isValid() ? &m_materialInstance->material : ZP_NULL;
}
zpMaterial* zpMaterialHandle::get()
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
    ZP_ASSERT( m_materialInstances.isEmpty(), "Material(s) still loaded: %d", m_materialInstances.size() );

    m_shaderManager = ZP_NULL;
    m_textureManager = ZP_NULL;
}

zp_bool zpMaterialManager::loadMaterial( const zp_char* materialFile, zpMaterialHandle& material )
{
    zpMaterialInstance* foundMaterialInstance = ZP_NULL;
    zpMaterialInstance** b = m_materialInstances.begin();
    zpMaterialInstance** e = m_materialInstances.end();
    for( ; b != e; ++b )
    {
        zpMaterialInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            ClearMaterial( t->material );

            foundMaterialInstance = t;
            break;
        }
        else if( zp_strcmp( materialFile, t->materialName.str() ) == 0 )
        {
            material.set( t->instanceId, t );
            return true;
        }
    }

    if( foundMaterialInstance == ZP_NULL )
    {
        foundMaterialInstance = new( g_globalAllocator->allocate( sizeof( zpMaterialInstance ) ) ) zpMaterialInstance;
        m_materialInstances.pushBack( foundMaterialInstance );
    }

    zpMaterialData materialData;
    LoadMaterialData( materialFile, materialData );

    ClearMaterial( foundMaterialInstance->material );

    FillMaterial( m_shaderManager, m_textureManager, materialData, foundMaterialInstance->material );

    foundMaterialInstance->lastModifiedTime = materialData.lastModifiedTime;
    foundMaterialInstance->refCount = 0;
    foundMaterialInstance->instanceId = ++m_newMaterialInstanceId;
    foundMaterialInstance->materialName = materialFile;

    material.set( foundMaterialInstance->instanceId, foundMaterialInstance );

    return true;
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
            ClearMaterial( t->material );

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
        foundMaterialInstance = new( g_globalAllocator->allocate( sizeof( zpMaterialInstance ) ) ) zpMaterialInstance;
        m_materialInstances.pushBack( foundMaterialInstance );
    }

    foundMaterialInstance->lastModifiedTime = ZP_MATERIAL_NOT_FILE;
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
            ClearMaterial( b->material );

            b->~zpMaterialInstance();

            g_globalAllocator->free( b );

            m_materialInstances.eraseAtSwapBack( i );

            --i;
            --imax;
        }
    }
}

void zpMaterialManager::reloadChangedMaterials()
{
    zpMaterialInstance** b = m_materialInstances.begin();
    zpMaterialInstance** e = m_materialInstances.end();
    for( ; b != e; ++b )
    {
        zpMaterialInstance* m = *b;
        if( m->refCount > 0 && m->lastModifiedTime != ZP_MATERIAL_NOT_FILE )
        {
            const zp_char* materialName = m->materialName.str();
            zp_time_t lastModTime = zpFile::lastModifiedTime( materialName );
            if( m->lastModifiedTime != lastModTime )
            {
                zp_int r;
                zpMaterialData materialData;
                r = LoadMaterialData( materialName, materialData );

                if( r )
                {
                    continue;
                }

                ClearMaterial( m->material );

                FillMaterial( m_shaderManager, m_textureManager, materialData, m->material );
            }
        }
    }
}
