#include "Engine/zpEngine.h"

zpObject::zpObject()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
    , m_layers( 0 )
    , m_tags( 0 )
    , m_flags( 0 )
{

}

zpObject::~zpObject()
{
    m_instanceId = ZP_HANDLE_ID_EMPTY;
}

zp_hash64 zpObject::getInstanceId() const
{
    return m_instanceId;
}

const zpString& zpObject::getName() const
{
    return m_name;
}

void zpObject::setName( const zp_char* name )
{
    m_name = name;
}


zp_ulong zpObject::getLayers() const
{
    return m_layers;
}

void zpObject::setLayers( zp_ulong layers )
{
    m_layers = layers;
}

void zpObject::markOnLayer( zp_int layerIndex, zp_bool onLayer )
{
    m_layers ^= ( ( onLayer ? -1 : 0 ) ^ m_layers ) & ( 1ULL << layerIndex );
}

zp_bool zpObject::isOnLayer( zp_int layerIndex ) const
{
    zp_ulong layer = 1ULL << layerIndex;
    return ( m_layers & layer ) == layer;
}

zp_ulong zpObject::getTags() const
{
    return m_tags;
}

void zpObject::setTags( zp_ulong tags )
{
    m_tags = tags;
}

void zpObject::markOnTag( zp_int tagIndex, zp_bool tagged )
{
    m_tags ^= ( ( tagged ? -1 : 0 ) ^ m_tags ) & ( 1ULL << tagIndex );
}

zp_bool zpObject::hasTag( zp_int tagIndex ) const
{
    zp_ulong tag = 1ULL << tagIndex;
    return ( m_tags & tag ) == tag;
}

const zpAllComponents* zpObject::getAllComponents() const
{
    return &m_allComponents;
}

zpAllComponents* zpObject::getAllComponents()
{
    return &m_allComponents;
}

void zpObject::setActive( zp_bool isActive )
{
    m_flags ^= ( ( isActive ? -1 : 0 ) ^ m_flags ) & ( 1ULL << ZP_OBJECT_FLAG_ACTIVE );
}

zp_bool zpObject::isActive() const
{
    return ( m_flags & ( 1ULL << ZP_OBJECT_FLAG_ACTIVE ) ) == ( 1ULL << ZP_OBJECT_FLAG_ACTIVE );
}

void zpObject::destroy()
{
    setActive( false );
    m_flags |= ( 1ULL << ZP_OBJECT_FLAG_SHOULD_DESTROY );
    m_instanceId = ZP_HANDLE_ID_EMPTY;
}

void zpObject::setInstanceId( zp_hash64 instanceId )
{
    m_instanceId = instanceId;
}

zp_bool zpObject::shouldDestroy() const
{
    return ( m_flags & ( 1ULL << ZP_OBJECT_FLAG_SHOULD_DESTROY ) ) == ( 1ULL << ZP_OBJECT_FLAG_SHOULD_DESTROY );
}
