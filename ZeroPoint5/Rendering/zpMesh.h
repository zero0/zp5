#pragma once
#ifndef ZP_MESH_H
#define ZP_MESH_H

struct zpMeshPart
{
    zp_size_t indexOffset;
    zp_size_t indexCount;
    zp_size_t vertexOffset;
    zp_size_t vertexCount;
    zpBoundingAABB boundingBox;
};

struct zpMesh
{
    zpVertexFormat vertexFormat;
    zp_size_t numMeshParts;
    zpRenderBuffer vertexData;
    zpRenderBuffer indexData;
    zpMeshPart parts[ 8 ];
};

struct zpMeshInstance
{
    zpString meshName;
    zp_size_t refCount;
    zp_hash64 instanceId;
    zp_time_t lastModifiedTime;
    zpMesh mesh;
};

class zpMeshHandle
{
public:
    zpMeshHandle();
    zpMeshHandle( const zpMeshHandle& other );
    zpMeshHandle( zpMeshHandle&& other );
    ~zpMeshHandle();

    zpMeshHandle& operator=( const zpMeshHandle& other );
    zpMeshHandle& operator=( zpMeshHandle&& other );

    const zpMesh* operator->() const;
    zpMesh* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpMeshInstance* meshInstance );

    zpMeshInstance* m_meshInstance;
    zp_hash64 m_instanceId;

    friend class zpMeshManager;
};

class zpMeshManager
{
public:
    zpMeshManager();
    ~zpMeshManager();

    void setup();
    void teardown();

    zp_bool loadMesh( const zp_char* meshFile, zpMeshHandle& mesh );
    zp_bool getMesh( const zp_char* meshName, zpMeshHandle& mesh );

    void garbageCollect();
    void reloadChangedMeshs();


private:
    zpVector< zpMeshInstance* > m_meshInstances;

    zp_hash64 m_newMeshInstanceId;
};

#endif // !ZP_MESH_H
