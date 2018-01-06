#include "Engine/zpEngine.h"

zpScene::zpScene()
    : m_instanceId( ZP_HANDLE_ID_EMPTY )
{

}

zpScene::~zpScene()
{
    destroyAllObjectsInScene();
}

zp_hash64 zpScene::getInstanceId() const
{
    return m_instanceId;
}

void zpScene::addObject( const zpObjectHandle& handle )
{
    m_objectsInScene.pushBackEmpty() = handle;
}

void zpScene::removeObject( const zpObjectHandle& handle )
{
    m_objectsInScene.eraseAll( handle );
}

void zpScene::removeAllObjects()
{
    m_objectsInScene.clear();
}

void zpScene::destroyAllObjectsInScene()
{
    zpObjectHandle* b = m_objectsInScene.begin();
    zpObjectHandle* e = m_objectsInScene.end();
    for( ; b != e; ++b )
    {
        b->get()->destroy();
    }
    m_objectsInScene.clear();
}

void zpScene::setInstanceId( zp_hash64 instanceId )
{
    m_instanceId = instanceId;
}

//
//
//

zpSceneHandle::zpSceneHandle()
    : m_instanceId( ZP_HANDLE_ID_INVALID )
    , m_sceneInstance( ZP_NULL )
{
}

zpSceneHandle::zpSceneHandle( const zpSceneHandle& other )
    : m_instanceId( other.m_instanceId )
    , m_sceneInstance( other.m_sceneInstance )
{
    addRef();
}

zpSceneHandle::zpSceneHandle( zpSceneHandle&& other )
    : m_instanceId( other.m_instanceId )
    , m_sceneInstance( other.m_sceneInstance )
{
    addRef();
    other.release();
}

zpSceneHandle::~zpSceneHandle()
{
    release();
}

zpSceneHandle& zpSceneHandle::operator=( const zpSceneHandle& other )
{
    set( other.m_instanceId, other.m_sceneInstance );

    return *this;
}
zpSceneHandle& zpSceneHandle::operator=( zpSceneHandle&& other )
{
    set( other.m_instanceId, other.m_sceneInstance );

    other.release();

    return *this;
}

const zpScene* zpSceneHandle::operator->() const
{
    return isValid() ? &m_sceneInstance->scene : ZP_NULL;
}
zpScene* zpSceneHandle::operator->()
{
    return isValid() ? &m_sceneInstance->scene : ZP_NULL;
}

zp_bool zpSceneHandle::isValid() const
{
    return m_sceneInstance != ZP_NULL && m_sceneInstance->scene.getInstanceId() == m_instanceId;
}

void zpSceneHandle::addRef()
{
    if( isValid() )
    {
        ++m_sceneInstance->refCount;
    }
}

void zpSceneHandle::releaseRef()
{
    if( isValid() )
    {
        ZP_ASSERT( m_sceneInstance->refCount != 0, "" );
        --m_sceneInstance->refCount;
    }
}

void zpSceneHandle::set( zp_hash64 instanceId, zpSceneInstance* sceneInstance )
{
    releaseRef();

    m_instanceId = instanceId;
    m_sceneInstance = sceneInstance;

    addRef();
}

void zpSceneHandle::release()
{
    releaseRef();

    m_instanceId = ZP_HANDLE_ID_INVALID;
    m_sceneInstance = ZP_NULL;
}

zp_bool zpSceneHandle::operator==( const zpSceneHandle& other ) const
{
    const zp_bool eq = m_instanceId == other.m_instanceId && m_sceneInstance == other.m_sceneInstance;
    return eq;
}

//
//
//

zpSceneManager::zpSceneManager()
    : m_newSceneInstanceId( ZP_HANDLE_ID_EMPTY )
{

}

zpSceneManager::~zpSceneManager()
{

}

void zpSceneManager::setup()
{

}

void zpSceneManager::teardown()
{

}

void zpSceneManager::createScene( zpSceneHandle& handle )
{
    zp_hash64 instanceId = ++m_newSceneInstanceId;

    zpSceneInstance* instance = static_cast<zpSceneInstance*>( g_globalAllocator.allocate( sizeof( zpSceneInstance ) ) );

    new ( &instance->scene ) zpScene();

    instance->scene.setInstanceId( instanceId );
    instance->refCount = 0;

    handle.set( instanceId, instance );

    m_activeScenes.pushBack( instance );
}

void zpSceneManager::loadScene( const zp_char* sceneName, zpSceneHandle& handle )
{
    ++m_newSceneInstanceId;
}

void zpSceneManager::markDontDestroyOnLoad( const zpObjectHandle& handle )
{

}

void zpSceneManager::unmarkDontDestroyOnLoad( const zpObjectHandle& handle )
{

}

void zpSceneManager::update( zp_float dt, zp_float rt )
{

}

void zpSceneManager::garbageCollect()
{
    for( zp_size_t i = 0, imax = m_activeScenes.size(); i != imax; ++i )
    {
        zpSceneInstance* b = m_activeScenes[ i ];
        if( b->refCount == 0 )
        {
            ( &b->scene )->~zpScene();
            g_globalAllocator.free( b );

            m_activeScenes.erase( i );

            --i;
            --imax;
        }
    }
}
