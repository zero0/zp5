#include "zpEngine.h"

const zp_hash64 ZP_OBJECT_ID_INVALID = (zp_hash64)-1;
const zp_hash64 ZP_OBJECT_ID_EMPTY = 0;

//
//
//

zpObject::zpObject()
    : m_instanceId( ZP_OBJECT_ID_EMPTY )
    , m_layers( 0 )
    , m_tags( 0 )
{

}

zpObject::~zpObject()
{
    m_instanceId = ZP_OBJECT_ID_EMPTY;
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

//
//
//

zpObjectHandle::zpObjectHandle()
    : m_instanceId( ZP_OBJECT_ID_INVALID )
    , m_objectInstance( ZP_NULL )
{
}

zpObjectHandle::zpObjectHandle( const zpObjectHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_objectInstance( other.m_objectInstance )
{
    addRef();
}

zpObjectHandle::zpObjectHandle( zpObjectHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_objectInstance( other.m_objectInstance )
{
    addRef();
    other.invalidate();
}

zpObjectHandle::~zpObjectHandle()
{
    invalidate();
}

zpObjectHandle& zpObjectHandle::operator=( const zpObjectHandle& other )
{
    set( other.m_instanceId, other.m_objectInstance );

    return *this;
}
zpObjectHandle& zpObjectHandle::operator=( zpObjectHandle&& other )
{
    set( other.m_instanceId, other.m_objectInstance );
    
    other.invalidate();

    return *this;
}

const zpObject* zpObjectHandle::operator->() const
{
    return isValid() ? &m_objectInstance->m_object : ZP_NULL;
}
zpObject* zpObjectHandle::operator->()
{
    return isValid() ? &m_objectInstance->m_object : ZP_NULL;
}

zp_bool zpObjectHandle::isValid() const
{
    return m_objectInstance != ZP_NULL && m_objectInstance->m_object.getInstanceId() == m_instanceId;
}

void zpObjectHandle::addRef()
{
    if( isValid() )
    {
        ++m_objectInstance->m_refCount;
    }
}

void zpObjectHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_objectInstance->m_refCount != 0, "" );
        --m_objectInstance->m_refCount;
    }
}

void zpObjectHandle::set( zp_hash64 instanceId, zpObjectInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_objectInstance = objectInstance;

    addRef();
}

void zpObjectHandle::invalidate()
{
    releaseRef();

    m_instanceId = ZP_OBJECT_ID_INVALID;
    m_objectInstance = ZP_NULL;
}

//
//
//

zpObjectManager::zpObjectManager()
    : m_objectMemory( sizeof( zpObjectInstance ) * 512 )
    , m_activeObjects( 512 )
    , m_newObjectInstanceId( ZP_OBJECT_ID_EMPTY )
{

}

zpObjectManager::~zpObjectManager()
{

}

void zpObjectManager::createObject( zpObjectHandle& objHandle )
{
    zp_hash64 instanceId = ++m_newObjectInstanceId;

    zpObjectInstance* instance = static_cast<zpObjectInstance*>( m_objectMemory.allocate( sizeof( zpObjectInstance ) ) );
    instance->m_object.setInstanceId( instanceId );
    instance->m_refCount = 0;

    objHandle.set( instanceId, instance );

    m_activeObjects.pushBack( instance );
}

void zpObjectManager::destryObject( zpObjectHandle& objHandle )
{
    objHandle.invalidate();
}

zp_bool zpObjectManager::findObjectByID( zp_hash64 instanceId, zpObjectHandle& handle ) const
{
    zp_bool found = false;

    zpObjectInstance* const* b = m_activeObjects.begin();
    zpObjectInstance* const* e = m_activeObjects.end();
    for( ; b != e; ++b )
    {
        zpObjectInstance* o = *b;
        if( o->m_object.getInstanceId() == instanceId )
        {
            handle.set( instanceId, o );
            found = true;
            break;
        }
    }

    return found;
}

void zpObjectManager::update()
{
    zpObjectInstance** b = m_activeObjects.begin();
    zpObjectInstance** e = m_activeObjects.end();
    for( ; b != e; ++b )
    {

    }
}

void zpObjectManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_activeObjects.size(); i != imax; ++i )
    {
        zpObjectInstance* b = m_activeObjects[ i ];
        if( b->m_refCount == 0 )
        {
            m_objectMemory.free( b );

            m_activeObjects.erase( i );

            --i;
            --imax;
        }
    }
}
