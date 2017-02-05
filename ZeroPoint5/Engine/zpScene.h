#pragma once
#ifndef ZP_SCENE_H
#define ZP_SCENE_H

class zpScene
{
public:
    zpScene();
    ~zpScene();

    void addObject( const zpObjectHandle& handle );
    void removeObject( const zpObjectHandle& handle );

private:
    zpVector< zpObjectHandle > m_objectsInScene;
};

struct zpSceneInstance
{
    zpScene scene;
    zp_size_t refCount;
    zp_hash64 m_instanceId;
    zpString m_sceneName;
};

class zpSceneManager
{
public:
    zpSceneManager();
    ~zpSceneManager();

    void loadScene( const zp_char* sceneName );

    void markDontDestroyOnLoad( const zpObjectHandle& handle );
    void unmarkDontDestroyOnLoad( const zpObjectHandle& handle );

    void update( zp_float dt, zp_float rt );

private:
    zpVector< zpObjectHandle > m_objectsDontDestroyOnLoad;
    zpVector< zpSceneInstance* > m_activeScenes;

    zp_hash64 m_newSceneInstanceId;
};

#endif // !ZP_SCENE_H
