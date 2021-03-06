#include "Engine/zpEngine.h"

zpObjectHandle::zpObjectHandle()
    : m_instanceId( ZP_HANDLE_ID_INVALID )
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
    other.release();
}

zpObjectHandle::~zpObjectHandle()
{
    release();
}

zpObjectHandle& zpObjectHandle::operator=( const zpObjectHandle& other )
{
    set( other.m_instanceId, other.m_objectInstance );

    return *this;
}
zpObjectHandle& zpObjectHandle::operator=( zpObjectHandle&& other )
{
    set( other.m_instanceId, other.m_objectInstance );

    other.release();

    return *this;
}

const zpObject* zpObjectHandle::get() const
{
    return isValid() ? &m_objectInstance->object : ZP_NULL;
}
zpObject* zpObjectHandle::get()
{
    return isValid() ? &m_objectInstance->object : ZP_NULL;
}

zp_bool zpObjectHandle::isValid() const
{
    const zp_bool isValid = m_objectInstance != ZP_NULL && m_objectInstance->object.getInstanceId() == m_instanceId;
    return isValid;
}

void zpObjectHandle::addRef()
{
    if( isValid() )
    {
        ++m_objectInstance->refCount;
    }
}

void zpObjectHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_objectInstance->refCount != 0, "" );
        --m_objectInstance->refCount;
    }
}

void zpObjectHandle::set( zp_hash64 instanceId, zpObjectInstance* objectInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_objectInstance = objectInstance;

    addRef();
}

void zpObjectHandle::release()
{
    releaseRef();

    m_instanceId = ZP_HANDLE_ID_INVALID;
    m_objectInstance = ZP_NULL;
}

zp_bool zpObjectHandle::operator==( const zpObjectHandle& other ) const
{
    const zp_bool eq = m_instanceId == other.m_instanceId && m_objectInstance == other.m_objectInstance;
    return eq;
}


//
//
//

zpObjectManager::zpObjectManager()
    : m_activeObjects( 4 )
    , m_newObjectInstanceId( ZP_HANDLE_ID_EMPTY )
{

}

zpObjectManager::~zpObjectManager()
{

}

void zpObjectManager::setup()
{

}

void zpObjectManager::teardown()
{

}

void zpObjectManager::createObject( zpObjectHandle& handle )
{
    zp_hash64 instanceId = ++m_newObjectInstanceId;

    zpObjectInstance* instance = static_cast<zpObjectInstance*>( g_globalAllocator->allocate( sizeof( zpObjectInstance ) ) );
   
    new ( &instance->object ) zpObject();
    
    instance->object.setInstanceId( instanceId );
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_activeObjects.pushBack( instance );
}

void zpObjectManager::destroyObject( zpObjectHandle& handle )
{
    if( handle.isValid() )
    {
        handle->destroy();

        handle.release();
    }
}

zp_bool zpObjectManager::findObjectByID( zp_hash64 instanceId, zpObjectHandle& handle ) const
{
    zp_bool found = false;

    zpObjectInstance* const* b = m_activeObjects.begin();
    zpObjectInstance* const* e = m_activeObjects.end();
    for( ; b != e; ++b )
    {
        zpObjectInstance* o = *b;
        if( o->object.getInstanceId() == instanceId )
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
        if( b->refCount == 0 || b->object.shouldDestroy() )
        {
            ( b )->~zpObjectInstance();
            g_globalAllocator->free( b );

            m_activeObjects.eraseAtSwapBack( i );

            --i;
            --imax;
        }
    }
}

const zpObjectInstance* const* zpObjectManager::beginActiveObjects() const
{
    return m_activeObjects.begin();
}

const zpObjectInstance* const* zpObjectManager::endActiveObjects() const
{
    return m_activeObjects.end();
}