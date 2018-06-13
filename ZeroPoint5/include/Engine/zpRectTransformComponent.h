#pragma once
#ifndef ZP_RECT_TRANSFORM_COMPONENT_H
#define ZP_RECT_TRANSFORM_COMPONENT_H

struct zpRectTransformComponentDesc
{
    zpRectf rect;
};

class zpRectTransformComponentManager;
class zpRectTransformComponent;

typedef zpHandleInstance< zpRectTransformComponent > zpRectTransformComponentInstance;
typedef zpHandle< zpRectTransformComponent > zpRectTransformComponentHandle;


class zpRectTransformComponentManager : public zpComponentManager< zpRectTransformComponent >
{
public:
    void createRectTransformComponent( handle_reference h, zpRectTransformComponentDesc* desc );

protected:

};

class zpRectTransformComponent : public zpComponent
{
public:
    const zpRectf& getRect() const;
    void setRect( const zpRectf& rect );
    void setSize( const zpVector2f& size );
    void setPosition( const zpVector2f& position );

    const zpColorf& getColor() const;
    zp_float getAlpha() const;
    zp_float getFinalAlpha() const;

    void setColorAlpha( const zpColorf& color );
    void setColor( const zpColorf& rgb );
    void setAlpha( zp_float alpha );

    void setParentRectTransform( const zpRectTransformComponentHandle& parent );
    const zpRectTransformComponentHandle& getParentRectTransform() const;

    void addChildRectTransform( const zpRectTransformComponentHandle& child );
    void removeChildRectTransform( const zpRectTransformComponentHandle& child );
    void removeAllChildRectTransforms();

    zp_size_t getChildRectTransformCount() const;
    const zpRectTransformComponentHandle& getChildRectTransform( zp_size_t index ) const;

protected:
    void onUpdate( zp_float dt, zp_float rt );

private:
    zpRectf m_rect;
    zpColorf m_color;
    zpString m_text;

    zpRectTransformComponentHandle m_parent;
    zpVector<zpRectTransformComponentHandle> m_children;
};

#endif // !ZP_RECTTRANSFORM_COMPONENT_H
