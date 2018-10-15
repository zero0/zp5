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

typedef zpHandleInstance< zpMeshRendererComponent > zpMeshRendererComponentInstance;
typedef zpHandle< zpMeshRendererComponent > zpMeshRendererComponentHandle;

//
//
//

class zpMeshRendererComponentManager : public zpComponentManager< zpMeshRendererComponent >
{
public:
    void createMeshRendererComponent( handle_reference handle, zpMeshRendererComponentDesc* desc );

    void render( zpRenderContext* ctx );
};

//
//
//

class zpMeshRendererComponent : public zpComponent
{
public:
    zpMeshRendererComponent();
    ~zpMeshRendererComponent();

    void render( zpRenderContext* ctx );

    void setMesh( const zpMeshHandle& mesh );
    const zpMeshHandle& getMesh() const;

    void setMaterial( const zpMaterialHandle& material );
    const zpMaterialHandle& getMaterial() const;

protected:
    void onUpdate( zp_float dt, zp_float rt );

private:
    zp_flag32 m_renderLayers;

    zpMeshHandle m_mesh;
    zpMaterialHandle m_material;

    friend class zpMeshRendererComponentManager;
};

#endif // !ZP_MESH_RENDERER_COMPONENT_H
