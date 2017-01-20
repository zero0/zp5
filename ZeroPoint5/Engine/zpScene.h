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
    zp_hash64 m_instanceId;

    zpString m_sceneName;
    zpVector< zpObjectHandle > m_objectsInScene;
};

struct zpSceneInstance
{
    zpScene scene;
    zp_size_t refCount;
};

class zpSceneManager
{
public:
    zpSceneManager();
    ~zpSceneManager();

    void loadScene( const zp_char* sceneName );

    void markDontDestroyOnLoad( const zpObjectHandle& handle );
    void unmarkDontDestroyOnLoad( const zpObjectHandle& handle );

private:
    zpVector< zpObjectHandle > m_objectsDontDestroyOnLoad;

    zp_hash64 m_newSceneInstanceId;
};

#endif // !ZP_SCENE_H
