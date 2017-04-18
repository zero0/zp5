#include "zpRendering.h"
#include <new>

const zp_hash64 ZP_MESH_ID_INVALID = static_cast<zp_hash64>( -1 );
const zp_hash64 ZP_MESH_ID_EMPTY = 0;
const zp_time_t ZP_MESH_NOT_FILE = static_cast<zp_time_t>( -1 );

struct zpMeshData
{
    zp_time_t lastModifiedTime;
};

static zp_int LoadMeshData( const zp_char* filepath, zpMeshData& meshData )
{
    //zpFile meshFile( filepath, ZP_FILE_MODE_BINARY_READ );
    //if( meshFile.getResult() != ZP_FILE_SUCCESS )
    //{
    //    return -1;
    //}

    return 0;
}

//
//
//

zpMeshHandle::zpMeshHandle()
    : m_instanceId( ZP_MESH_ID_INVALID )
    , m_meshInstance( ZP_NULL )
{
}

zpMeshHandle::zpMeshHandle( const zpMeshHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_meshInstance( other.m_meshInstance )
{
    addRef();
}

zpMeshHandle::zpMeshHandle( zpMeshHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_meshInstance( other.m_meshInstance )
{
    addRef();
    other.release();
}

zpMeshHandle::~zpMeshHandle()
{
    release();
}

zpMeshHandle& zpMeshHandle::operator=( const zpMeshHandle& other )
{
    set( other.m_instanceId, other.m_meshInstance );

    return *this;
}
zpMeshHandle& zpMeshHandle::operator=( zpMeshHandle&& other )
{
    set( other.m_instanceId, other.m_meshInstance );

    other.release();

    return *this;
}

const zpMesh* zpMeshHandle::operator->() const
{
    return isValid() ? &m_meshInstance->mesh : ZP_NULL;
}
zpMesh* zpMeshHandle::operator->()
{
    return isValid() ? &m_meshInstance->mesh : ZP_NULL;
}

zp_bool zpMeshHandle::isValid() const
{
    return m_meshInstance != ZP_NULL && m_meshInstance->instanceId == m_instanceId;
}

void zpMeshHandle::release()
{
    releaseRef();

    m_instanceId = ZP_MESH_ID_INVALID;
    m_meshInstance = ZP_NULL;
}

void zpMeshHandle::addRef()
{
    if( isValid() )
    {
        ++m_meshInstance->refCount;
    }
}

void zpMeshHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_meshInstance->refCount != 0, "" );
        --m_meshInstance->refCount;
    }
}

void zpMeshHandle::set( zp_hash64 instanceId, zpMeshInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_meshInstance = objectInstance;

    addRef();
}

//
//
//

zpMeshManager::zpMeshManager()
    : m_meshInstances( 64 )
    , m_newMeshInstanceId( ZP_MESH_ID_EMPTY )
{
}
zpMeshManager::~zpMeshManager()
{
}

void zpMeshManager::setup()
{
}

void zpMeshManager::teardown()
{
}

zp_bool zpMeshManager::loadMesh( const zp_char* meshFile, zpMeshHandle& mesh )
{
    zpMeshInstance* foundMeshInstance = ZP_NULL;
    zpMeshInstance** b = m_meshInstances.begin();
    zpMeshInstance** e = m_meshInstances.end();
    for( ; b != e; ++b )
    {
        zpMeshInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            foundMeshInstance = t;
            break;
        }
        else if( zp_strcmp( meshFile, t->meshName.str() ) == 0 )
        {
            mesh.set( t->instanceId, t );
            return true;
        }
    }

    if( foundMeshInstance == ZP_NULL )
    {
        foundMeshInstance = new( g_globalAllocator.allocate( sizeof( zpMeshInstance ) ) ) zpMeshInstance;
        m_meshInstances.pushBack( foundMeshInstance );
    }

    zpMeshData meshData;
    LoadMeshData( meshFile, meshData );

    foundMeshInstance->lastModifiedTime = meshData.lastModifiedTime;
    foundMeshInstance->refCount = 0;
    foundMeshInstance->instanceId = ++m_newMeshInstanceId;
    foundMeshInstance->meshName = meshFile;

    mesh.set( foundMeshInstance->instanceId, foundMeshInstance );

    return true;
}

zp_bool zpMeshManager::getMesh( const zp_char* meshName, zpMeshHandle& mesh )
{
    zpMeshInstance* foundMeshInstance = ZP_NULL;
    zpMeshInstance** b = m_meshInstances.begin();
    zpMeshInstance** e = m_meshInstances.end();
    for( ; b != e; ++b )
    {
        zpMeshInstance* t = ( *b );
        if( t->refCount == 0 )
        {
            foundMeshInstance = t;
            break;
        }
        else if( zp_strcmp( meshName, t->meshName.str() ) == 0 )
        {
            mesh.set( t->instanceId, t );
            return true;
        }
    }

    if( foundMeshInstance == ZP_NULL )
    {
        foundMeshInstance = new( g_globalAllocator.allocate( sizeof( zpMeshInstance ) ) ) zpMeshInstance;
        m_meshInstances.pushBack( foundMeshInstance );
    }

    foundMeshInstance->lastModifiedTime = ZP_MESH_NOT_FILE;
    foundMeshInstance->refCount = 0;
    foundMeshInstance->instanceId = ++m_newMeshInstanceId;
    foundMeshInstance->meshName = meshName;

    mesh.set( foundMeshInstance->instanceId, foundMeshInstance );

    return true;
}

void zpMeshManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_meshInstances.size(); i != imax; ++i )
    {
        zpMeshInstance* b = m_meshInstances[ i ];
        if( b->refCount == 0 )
        {
            b->~zpMeshInstance();

            g_globalAllocator.free( b );

            m_meshInstances.erase( i );

            --i;
            --imax;
        }
    }
}

void zpMeshManager::reloadChangedMeshs()
{
    zpMeshInstance** b = m_meshInstances.begin();
    zpMeshInstance** e = m_meshInstances.end();
    for( ; b != e; ++b )
    {
        zpMeshInstance* m = *b;
        if( m->refCount > 0 && m->lastModifiedTime != ZP_MESH_NOT_FILE )
        {
            const zp_char* meshName = m->meshName.str();
            zp_time_t lastModTime = zpFile::lastModifiedTime( meshName );
            if( m->lastModifiedTime != lastModTime )
            {
                zp_int r;
                zpMeshData meshData;
                r = LoadMeshData( meshName, meshData );

                if( r )
                {
                    continue;
                }

            }
        }
    }
}
