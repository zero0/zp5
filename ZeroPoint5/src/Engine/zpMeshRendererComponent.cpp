#include "Engine/zpEngine.h"
//#include <new>

zpMeshRendererComponent::zpMeshRendererComponent()
{
}

zpMeshRendererComponent::~zpMeshRendererComponent()
{
}

void zpMeshRendererComponent::render( zpRenderContext* ctx )
{
    ZP_PROFILER_BLOCK();

    if( m_mesh.isValid() && m_material.isValid() )
    {
        zpMatrix4f localToWorld;
        zpTransformComponentHandle& transform = getParentObject()->getAllComponents()->transform;
        if( transform.isValid() )
        {
            transform->getWorldMatrix();
        }
        else
        {
            zpMath::MatrixStore4( zpMath::MatrixIdentity(), localToWorld.m );
        }

        zpDrawRenderable desc = {};
        desc.renderLayers = m_renderLayers;
        desc.material = m_material;
        desc.mesh = m_mesh;
        desc.localToWorld = localToWorld;
        desc.passIndex = 0;
        desc.subMeshIndex = -1;
        desc.bounds = m_mesh->bounds;

        ctx->addDrawRenderable( desc );
    }
}

void zpMeshRendererComponent::setMesh( const zpMeshHandle& mesh )
{
    m_mesh = mesh;
}
const zpMeshHandle& zpMeshRendererComponent::getMesh() const
{
    return m_mesh;
}

void zpMeshRendererComponent::setMaterial( const zpMaterialHandle& material )
{
    m_material = material;
}
const zpMaterialHandle& zpMeshRendererComponent::getMaterial() const
{
    return m_material;
}

void zpMeshRendererComponent::onUpdate( zp_float dt, zp_float rt )
{

}

//
//
//

void zpMeshRendererComponentManager::render( zpRenderContext* ctx )
{
    ZP_PROFILER_BLOCK();
    
    zpVector< instance_pointer >::iterator b = m_instances.begin();
    zpVector< instance_pointer >::iterator e = m_instances.end();

    for( ; b != e; ++b )
    {
        zpVector< instance_pointer >::value_type p = *b;
        if( p->component.isEnabled() )
        {
            p->component.render( ctx );
        }
    }
}

void zpMeshRendererComponentManager::createMeshRendererComponent( handle_reference handle, zpMeshRendererComponentDesc* desc )
{
    createComponent( handle );

    if( desc )
    {

    }
}
