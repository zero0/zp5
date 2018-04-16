#pragma once
#ifndef ZP_MESH_RENDERER_COMPONENT_H
#define ZP_MESH_RENDERER_COMPONENT_H

struct zpMeshRendererComponentDesc
{
    zpMeshHandle mesh;
    zpMaterialHandle material;
};

class zpMeshRendererComponentManager;
class zpMeshRendererComponent;

typedef zpComponentInstance< zpMeshRendererComponent > zpMeshRendererComponentInstance;
typedef zpComponentHandle< zpMeshRendererComponent, zpMeshRendererComponentInstance, zpMeshRendererComponentManager > zpMeshRendererComponentHandle;

//
//
//

class zpMeshRendererComponentManager : public zpComponentManager< zpMeshRendererComponent, zpMeshRendererComponentInstance, zpMeshRendererComponentManager, zpMeshRendererComponentHandle >
{
public:
    void createMeshRendererComponent( handle_reference handle, zpMeshRendererComponentDesc* desc );

    void render( zpRenderingContext* ctx );
};

//
//
//

class zpMeshRendererComponent : public zpComponent< zpMeshRendererComponentManager >
{
public:
    zpMeshRendererComponent();
    ~zpMeshRendererComponent();

    void render( zpRenderingContext* ctx );

    void setMesh( const zpMeshHandle& mesh );
    const zpMeshHandle& getMesh() const;

    void setMaterial( const zpMaterialHandle& material );
    const zpMaterialHandle& getMaterial() const;

protected:
    void onUpdate( zp_float dt, zp_float rt );

private:
    zp_byte m_renderLayer;

    zpMeshHandle m_mesh;
    zpMaterialHandle m_material;

    friend class zpMeshRendererComponentManager;
};

#endif // !ZP_MESH_RENDERER_COMPONENT_H
