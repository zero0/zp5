#include "Rendering/zpRendering.h"
#include <new>

const zp_hash64 ZP_MESH_ID_INVALID = static_cast<zp_hash64>( -1 );
const zp_hash64 ZP_MESH_ID_EMPTY = 0;
const zp_time_t ZP_MESH_NOT_FILE = static_cast<zp_time_t>( -1 );

struct zpMeshData
{
    zp_time_t lastModifiedTime;
    zpVertexFormat vertexFormat;
    zpDataBuffer vertexBuffer;
    zpDataBuffer indexBuffer;
    zp_size_t numMeshParts;
    zpMeshPart parts[ ZP_MESH_MAX_MESH_PARTS ];
};

static zp_int LoadMeshData( const zp_char* filepath, zpMeshData& meshData )
{
    //zpFile meshFile( filepath, ZP_FILE_MODE_BINARY_READ );
    //if( meshFile.getResult() != ZP_FILE_SUCCESS )
    //{
    //    return -1;
    //}

    zpRectf rect = { 0, 0, 10, 10 };

    zp_ushort index[] = {
        0, 1, 2,
        2, 3, 0
    };

    struct Vertex
    {
        zpVector4fData p;
        zpColor32i c;
        zpVector2f u;
    };

    zpVector4fData p0 = { rect.x,               rect.y + rect.height, 0, 1 };
    zpVector4fData p1 = { rect.x,               rect.y, 0, 1 };
    zpVector4fData p2 = { rect.x + rect.height, rect.y, 0, 1 };
    zpVector4fData p3 = { rect.x + rect.height, rect.y + rect.height, 0, 1 };

    zpVector2f uv0 = { 0, 1 };
    zpVector2f uv1 = { 0, 0 };
    zpVector2f uv2 = { 1, 0 };
    zpVector2f uv3 = { 1, 1 };

    Vertex vert[] = {
        { p0, zpColor32::Red, uv0 },
        { p1, zpColor32::Green, uv1 },
        { p2, zpColor32::Blue, uv2 },
        { p3, zpColor32::White, uv3 },
    };

    meshData.numMeshParts = 1;
    meshData.vertexFormat = ZP_VERTEX_FORMAT_VERTEX_COLOR_UV;
    meshData.indexBuffer.write( index, 0, sizeof( index ) );
    meshData.vertexBuffer.write( vert, 0, sizeof( vert ) );
    zpMeshPart& part = meshData.parts[ 0 ];
    part.indexCount = 6;
    part.indexOffset = 0;
    part.vertexCount = 4;
    part.vertexOffset = 0;

    return 0;
}

static zp_int BuildMeshFromData( const zpMeshData& meshData, zpMesh& mesh, zpRenderingEngine* renderingEngine )
{
    mesh.vertexFormat = meshData.vertexFormat;
    mesh.numMeshParts = meshData.numMeshParts;
    zp_memset( mesh.parts, 0, sizeof( mesh.parts ) );

    for( zp_size_t i = 0; i < meshData.numMeshParts; ++i )
    {
        mesh.parts[ i ] = meshData.parts[ i ];
    }

    renderingEngine->createRenderBuffer( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_IMMUTABLE, meshData.vertexBuffer.getLength(), meshData.vertexBuffer.getBuffer(), mesh.vertexData );
    renderingEngine->createRenderBuffer( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_IMMUTABLE, meshData.indexBuffer.getLength(), meshData.indexBuffer.getBuffer(), mesh.indexData );
   
    return 0;
}


static zp_int DestroyMesh( zpMesh& mesh, zpRenderingEngine* renderingEngine )
{
    renderingEngine->destoryRenderBuffer( mesh.vertexData );
    renderingEngine->destoryRenderBuffer( mesh.indexData );

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

const zpMesh* zpMeshHandle::get() const
{
    return isValid() ? &m_meshInstance->mesh : ZP_NULL;
}
zpMesh* zpMeshHandle::get()
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
            DestroyMesh( t->mesh, m_renderingEngine );

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
        foundMeshInstance = new( g_globalAllocator->allocate( sizeof( zpMeshInstance ) ) ) zpMeshInstance;
        m_meshInstances.pushBack( foundMeshInstance );
    }

    zpMeshData meshData;
    LoadMeshData( meshFile, meshData );

    foundMeshInstance->lastModifiedTime = meshData.lastModifiedTime;
    foundMeshInstance->refCount = 0;
    foundMeshInstance->instanceId = ++m_newMeshInstanceId;
    foundMeshInstance->meshName = meshFile;

    BuildMeshFromData( meshData, foundMeshInstance->mesh, m_renderingEngine );

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
            DestroyMesh( t->mesh, m_renderingEngine );

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
        foundMeshInstance = new( g_globalAllocator->allocate( sizeof( zpMeshInstance ) ) ) zpMeshInstance;
        m_meshInstances.pushBack( foundMeshInstance );
    }

    zpMeshData meshData;
    LoadMeshData( "", meshData );

    foundMeshInstance->lastModifiedTime = ZP_MESH_NOT_FILE;
    foundMeshInstance->refCount = 0;
    foundMeshInstance->instanceId = ++m_newMeshInstanceId;
    foundMeshInstance->meshName = meshName;

    BuildMeshFromData( meshData, foundMeshInstance->mesh, m_renderingEngine );

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
            DestroyMesh( b->mesh, m_renderingEngine );

            b->~zpMeshInstance();

            g_globalAllocator->free( b );

            m_meshInstances.eraseAtSwapBack( i );

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
                r = DestroyMesh( m->mesh, m_renderingEngine );

                zpMeshData meshData;
                r = LoadMeshData( meshName, meshData );

                if( r )
                {
                    continue;
                }

                r = BuildMeshFromData( meshData, m->mesh, m_renderingEngine );
            }
        }
    }
}
