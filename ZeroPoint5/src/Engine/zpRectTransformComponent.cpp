#include "Engine/zpEngine.h"

enum zpRectTransformComponentFlags
{
    ZP_RECT_TRANSFORM_COLOR_DIRTY = ZP_COMPONENT_FLAG_USER_START,
    ZP_RECT_TRANSFORM_HIERARCHY_DIRTY,

};

void zpRectTransformComponentManager::createRectTransformComponent( handle_reference h, zpRectTransformComponentDesc* desc )
{
    createComponent( h );

    if( desc )
    {

    }
}

//
//
//

const zpRectf& zpRectTransformComponent::getRect() const
{
    return m_rect;
}

void zpRectTransformComponent::setRect( const zpRectf& rect )
{
    m_rect = rect;
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpRectTransformComponent::setSize( const zpVector2f& size )
{
    m_rect.width = size.x;
    m_rect.height = size.y;
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpRectTransformComponent::setPosition( const zpVector2f& position )
{
    m_rect.x = position.x;
    m_rect.y = position.y;
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

const zpColorf& zpRectTransformComponent::getColor() const
{
    return m_color;
}

zp_float zpRectTransformComponent::getAlpha() const
{
    return m_color.a;
}

zp_float zpRectTransformComponent::getFinalAlpha() const
{
    zp_float a = m_color.a;

    zpRectTransformComponentHandle p = m_parent;
    while( p.isValid() )
    {
        a *= p->getAlpha();

        p = p->getParentRectTransform();
    }

    return a;
}

void zpRectTransformComponent::setColorAlpha( const zpColorf& color )
{
    m_color.r = color.r;
    m_color.g = color.g;
    m_color.b = color.b;
    m_color.a = zp_clamp01( color.a );
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpRectTransformComponent::setColor( const zpColorf& rgb )
{
    m_color.r = rgb.r;
    m_color.g = rgb.g;
    m_color.b = rgb.b;
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpRectTransformComponent::setAlpha( zp_float alpha )
{
    m_color.a = zp_clamp01( alpha );
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

void zpRectTransformComponent::setParentRectTransform( const zpRectTransformComponentHandle& parent )
{
    m_parent = parent;
    markFlag( ZP_COMPONENT_FLAG_DIRTY );
}

const zpRectTransformComponentHandle& zpRectTransformComponent::getParentRectTransform() const
{
    return m_parent;
}

void zpRectTransformComponent::addChildRectTransform( const zpRectTransformComponentHandle& child )
{

}

void zpRectTransformComponent::removeChildRectTransform( const zpRectTransformComponentHandle& child )
{

}

void zpRectTransformComponent::removeAllChildRectTransforms()
{

}

zp_size_t zpRectTransformComponent::getChildRectTransformCount() const
{
    return m_children.size();
}

const zpRectTransformComponentHandle& zpRectTransformComponent::getChildRectTransform( zp_size_t index ) const
{
    return m_children[ index ];
}

void zpRectTransformComponent::onUpdate( zp_float dt, zp_float rt )
{
    if( isFlagSet( ZP_COMPONENT_FLAG_DIRTY ) )
    {
        clearFlag( ZP_COMPONENT_FLAG_DIRTY );
    }
}
