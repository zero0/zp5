#pragma once
#ifndef ZP_SHADER_MANAGER_H
#define ZP_SHADER_MANAGER_H

struct zpShaderInstance
{
    zpShader shader;
    zp_size_t refCount;
    zp_hash64 instanceId;
    zpString shaderName;
};


class zpShaderHandle
{
public:
    zpShaderHandle();
    zpShaderHandle( const zpShaderHandle& other );
    zpShaderHandle( zpShaderHandle&& other );
    ~zpShaderHandle();

    zpShaderHandle& operator=( const zpShaderHandle& other );
    zpShaderHandle& operator=( zpShaderHandle&& other );

    const zpShader* operator->() const;
    zpShader* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpShaderInstance* objectInstance );

    zpShaderInstance* m_shaderInstance;
    zp_hash64 m_instanceId;

    friend class zpShaderManager;
};

class zpShaderManager
{
public:
    zpShaderManager();
    ~zpShaderManager();

    void setup( zpRenderingEngine* engine );
    void teardown();

    zp_bool getShader( const zp_char* shaderName, zpShaderHandle& shader );

    void garbageCollect();

private:
    zpBlockAllocator m_shaderMemory;
    zpVector< zpShaderInstance* > m_shaderInstances;

    zpRenderingEngine* m_engine;
    zp_hash64 m_newShaderInstanceId;
};

#endif // !ZP_SHADER_MANAGER_H
