#pragma once
#ifndef ZP_TEXTURE_MANAGER_H
#define ZP_TEXTURE_MANAGER_H

struct zpTextureInstance
{
    zpString textureName;
    zp_size_t refCount;
    zp_time_t lastModifiedTime;
    zp_hash64 instanceId;
    zpTexture texture;
};

class zpTextureHandle
{
public:
    zpTextureHandle();
    zpTextureHandle( const zpTextureHandle& other );
    zpTextureHandle( zpTextureHandle&& other );
    ~zpTextureHandle();

    zpTextureHandle& operator=( const zpTextureHandle& other );
    zpTextureHandle& operator=( zpTextureHandle&& other );

    const zpTexture* operator->() const;
    zpTexture* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpTextureInstance* objectInstance );

    zpTextureInstance* m_textureInstance;
    zp_hash64 m_instanceId;

    friend class zpTextureManager;
};

class zpTextureManager
{
public:
    zpTextureManager();
    ~zpTextureManager();

    void setup( zpRenderingEngine* engine );
    void teardown();

    zp_bool getTexture( const zp_char* textureName, zpTextureHandle& texture );
    zp_bool loadTexture( const zp_char* textureFile, zpTextureHandle& texture );

    void garbageCollect();
    void reloadChangedTextures();

private:
    zpVector< zpTextureInstance* > m_textureInstances;

    zpRenderingEngine* m_engine;
    zp_hash64 m_newTextureInstanceId;
};

#endif // !ZP_TEXTURE_MANAGER_H
