#pragma once
#ifndef ZP_TEXTURE_MANAGER_H

struct zpTextureInstance
{
    zpTexture texture;
    zp_size_t refCount;
    zp_hash64 instanceId;
    zpString textureName;
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

    void garbageCollect();

private:
    zpBlockAllocator m_textureMemory;
    zpVector< zpTextureInstance* > m_textureInstances;

    zpRenderingEngine* m_engine;
    zp_hash64 m_newTextureInstanceId;
};

#endif // !ZP_TEXTURE_MANAGER_H
