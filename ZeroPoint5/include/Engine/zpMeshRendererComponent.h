#pragma once
#ifndef ZP_MESH_RENDERER_COMPONENT_H
#define ZP_MESH_RENDERER_COMPONENT_H

class  zpMeshRendererComponent;
struct zpMeshRendererComponentInstance;

class zpMeshRendererComponentHandle
{
public:
    zpMeshRendererComponentHandle();
    zpMeshRendererComponentHandle( const zpMeshRendererComponentHandle& other );
    zpMeshRendererComponentHandle( zpMeshRendererComponentHandle&& other );
    ~zpMeshRendererComponentHandle();

    zpMeshRendererComponentHandle& operator=( const zpMeshRendererComponentHandle& other );
    zpMeshRendererComponentHandle& operator=( zpMeshRendererComponentHandle&& other );

    ZP_FORCE_INLINE const zpMeshRendererComponent* operator->() const
    {
        return get();
    }
    ZP_FORCE_INLINE zpMeshRendererComponent* operator->()
    {
        return get();
    }

    const zpMeshRendererComponent* get() const;
    zpMeshRendererComponent* get();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpMeshRendererComponentInstance* objectInstance );

    zp_hash64 m_instanceId;
    zpMeshRendererComponentInstance* m_meshRendererInstance;

    friend class zpMeshRendererComponentManager;
};

class zpMeshRendererComponent
{
public:
    zpMeshRendererComponent();
    ~zpMeshRendererComponent();

    void setParentObject( const zpObjectHandle& parent );
    const zpObjectHandle& getParentObject() const;

    void update( zp_float dt, zp_float rt );
    void render( zpRenderingContext* ctx );

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

    zp_hash64 getInstanceId() const;

    void setMesh( const zpMeshHandle& mesh );
    const zpMeshHandle& getMesh() const;

    void setMaterial( const zpMaterialHandle& material );
    const zpMaterialHandle& getMaterial() const;

private:
    void setInstanceId( zp_hash64 instanceId );

    zp_hash64 m_instanceID;
    zp_ulong m_flags;
    zp_byte m_renderLayer;

    zpObjectHandle m_parentObject;
    zpMeshHandle m_mesh;
    zpMaterialHandle m_material;

    friend class zpMeshRendererComponentManager;
};

struct zpMeshRendererComponentInstance
{
    zpMeshRendererComponent meshRenderer;
    zp_size_t refCount;
};

struct zpMeshRendererComponentDesc
{
    zpObjectHandle parentObject;
    zpMeshHandle mesh;
    zpMaterialHandle material;
};

class zpMeshRendererComponentManager
{
public:
    zpMeshRendererComponentManager();
    ~zpMeshRendererComponentManager();

    void update( zp_float dt, zp_float rt );
    void render( zpRenderingContext* ctx );

    void createMeshRendererComponent( zpMeshRendererComponentHandle& handle );
    void createMeshRendererComponent( zpMeshRendererComponentHandle& handle, zpMeshRendererComponentDesc* desc );

    void garbageCollect();

    zp_bool findComponentByID( zp_hash64 instanceId, zpMeshRendererComponentHandle& handle ) const;

private:
    zpVector< zpMeshRendererComponentInstance* > m_activeComponents;
    zp_size_t m_newMeshRendererComponentInstanceId;
};

#endif // !ZP_MESH_RENDERER_COMPONENT_H
