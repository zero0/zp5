#pragma once
#ifndef ZP_SCENE_H
#define ZP_SCENE_H

class zpScene
{
public:
    zpScene();
    ~zpScene();

    zp_hash64 getInstanceId() const;

    void addObject( const zpObjectHandle& handle );
    void removeObject( const zpObjectHandle& handle );

    void removeAllObjects();

    void destroyAllObjectsInScene();

private:
    void setInstanceId( zp_hash64 instanceId );

    zp_hash64 m_instanceId;

    zpVector< zpObjectHandle > m_objectsInScene;

    friend class zpSceneManager;
};

struct zpSceneInstance
{
    zpScene scene;
    zp_size_t refCount;
    zp_hash64 m_instanceId;
    zpString m_sceneName;
};

class zpSceneHandle
{
public:
    zpSceneHandle();
    zpSceneHandle( const zpSceneHandle& other );
    zpSceneHandle( zpSceneHandle&& other );
    ~zpSceneHandle();

    zpSceneHandle& operator=( const zpSceneHandle& other );
    zpSceneHandle& operator=( zpSceneHandle&& other );

    const zpScene* operator->() const;
    zpScene* operator->();

    zp_bool isValid() const;
    void release();

    zp_bool operator==( const zpSceneHandle& other ) const;

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpSceneInstance* sceneInstance );

    zp_hash64 m_instanceId;
    zpSceneInstance* m_sceneInstance;

    friend class zpSceneManager;
};

class zpSceneManager
{
public:
    zpSceneManager();
    ~zpSceneManager();

    void setup();
    void teardown();

    void createScene( zpSceneHandle& handle );
    void loadScene( const zp_char* sceneName, zpSceneHandle& handle );

    void markDontDestroyOnLoad( const zpObjectHandle& handle );
    void unmarkDontDestroyOnLoad( const zpObjectHandle& handle );

    void update( zp_float dt, zp_float rt );

    void garbageCollect();

private:
    zpVector< zpObjectHandle > m_objectsDontDestroyOnLoad;
    zpVector< zpSceneInstance* > m_activeScenes;

    zp_hash64 m_newSceneInstanceId;
};

#endif // !ZP_SCENE_H
