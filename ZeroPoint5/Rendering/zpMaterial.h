#pragma once
#ifndef ZP_MATERIAL_MANAGER_H
#define ZP_MATERIAL_MANAGER_H

struct zpMaterial
{
    zpColorf color;

    zpVector4fData mainTexST;
    zpVector4fData specTexST;
    zpVector4fData normTexST;

    zpShaderHandle shader;
    zpTextureHandle mainTex;
    zpTextureHandle specTex;
    zpTextureHandle normTex;
};

struct zpMaterialInstance
{
    zpString materialName;
    zp_size_t refCount;
    zp_hash64 instanceId;
    zp_time_t lastModifiedTime;
    zpMaterial material;
};

class zpMaterialHandle
{
public:
    zpMaterialHandle();
    zpMaterialHandle( const zpMaterialHandle& other );
    zpMaterialHandle( zpMaterialHandle&& other );
    ~zpMaterialHandle();

    zpMaterialHandle& operator=( const zpMaterialHandle& other );
    zpMaterialHandle& operator=( zpMaterialHandle&& other );

    const zpMaterial* operator->() const;
    zpMaterial* operator->();

    zp_bool isValid() const;
    void release();

private:
    void addRef();
    void releaseRef();

    void set( zp_hash64 instanceId, zpMaterialInstance* materialInstance );

    zpMaterialInstance* m_materialInstance;
    zp_hash64 m_instanceId;

    friend class zpMaterialManager;
};

class zpMaterialManager
{
public:
    zpMaterialManager();
    ~zpMaterialManager();

    void setup( zpShaderManager* shaderManager, zpTextureManager* textureManager );
    void teardown();

    zp_bool loadMaterial( const zp_char* materialFile, zpMaterialHandle& material );
    zp_bool getMaterial( const zp_char* materialName, zpMaterialHandle& material );

    void garbageCollect();
    void reloadChangedMaterials();


private:
    zpVector< zpMaterialInstance* > m_materialInstances;

    zpShaderManager* m_shaderManager;
    zpTextureManager* m_textureManager;

    zp_hash64 m_newMaterialInstanceId;
};

#endif // !ZP_MATERIAL_H
