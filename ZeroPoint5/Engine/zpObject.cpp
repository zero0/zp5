#include "zpEngine.h"

zpObject::zpObject()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
    , m_layers( 0 )
    , m_tags( 0 )
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

void zpObject::setInstanceId( zp_hash64 instanceId )
{
    m_instanceId = instanceId;
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
    if( onLayer )
    {
        m_layers |= ( 1ULL << layerIndex );
    }
    else
    {
        m_layers &= ~( 1ULL << layerIndex );
    }
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
    if( tagged )
    {
        m_tags |= ( 1ULL << tagIndex );
    }
    else
    {
        m_tags &= ~( 1ULL << tagIndex );
    }
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
