#include "Engine/zpEngine.h"
#include <new>

zpMeshRendererComponent::zpMeshRendererComponent()
{
}

zpMeshRendererComponent::~zpMeshRendererComponent()
{
}

void zpMeshRendererComponent::render( zpRenderingContext* ctx )
{
    if( m_mesh.isValid() && m_material.isValid() )
    {
        const zpMatrix4fData& transform = getParentObject()->getAllComponents()->transform->getWorldMatrix();

        ctx->drawMesh( m_renderLayer, transform, m_mesh, m_material );
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

void zpMeshRendererComponentManager::render( zpRenderingContext* ctx )
{
    ZP_PROFILER_BLOCK();
    
    zpVector< instance_pointer >::iterator b = m_activeComponents.begin();
    zpVector< instance_pointer >::iterator e = m_activeComponents.end();

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
