#include "Engine/zpEngine.h"


void zpRectTransformComponentManager::createRectTransformComponent( handle_reference h, zpRectTransformComponentDesc* desc )
{
    createComponent( h );
    h.addRef();
    if( desc )
    {

    }
}
