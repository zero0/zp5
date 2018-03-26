#pragma once
#ifndef ZP_RECT_TRANSFORM_COMPONENT_H
#define ZP_RECT_TRANSFORM_COMPONENT_H

struct zpRectTransformComponentDesc
{
    zpRectf rect;
};

class zpRectTransformComponentManager;
class zpRectTransformComponent;

typedef zpComponentInstance< zpRectTransformComponent > zpRectTransformComponentInstance;
typedef zpComponentHandle< zpRectTransformComponent, zpRectTransformComponentInstance, zpRectTransformComponentManager > zpRectTransformComponentHandle;


class zpRectTransformComponentManager : public zpComponentManager< zpRectTransformComponent, zpRectTransformComponentInstance, zpRectTransformComponentManager, zpRectTransformComponentHandle >
{
public:
    void createRectTransformComponent( handle_reference h, zpRectTransformComponentDesc* desc );

protected:

};

class zpRectTransformComponent : public zpComponent< zpRectTransformComponentManager >
{
protected:

};

#endif // !ZP_RECTTRANSFORM_COMPONENT_H
