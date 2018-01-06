#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

struct zpMaterialPartColor
{
    zpString name;
    zpColorf color;
};

struct zpMaterialPartVector
{
    zpString name;
    zpVector4fData vector;
};

struct zpMaterialPartTexture
{
    zpString name;
    zpTextureHandle texture;
};

struct zpMaterialPartMatrix
{
    zpString name;
    zpMatrix4fData matrix;
};

struct zpMaterialOld
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

struct zpMaterial
{
    zpShaderHandle shader;

    zpVector< zpMaterialPartColor > colors;
    zpVector< zpMaterialPartVector > vectors;
    zpVector< zpMaterialPartTexture > textures;
    zpVector< zpMaterialPartMatrix > matrices;

    void addColor( const zp_char* name, const zpColorf& color )
    {
        zpMaterialPartColor& p = colors.pushBackEmpty();
        p.name = name;
        p.color = color;
    }

    void addVector( const zp_char* name, const zpVector4fData& vector )
    {
        zpMaterialPartVector& p = vectors.pushBackEmpty();
        p.name = name;
        p.vector = vector;
    }

    void addTexture( const zp_char* name, const zpTextureHandle& texture )
    {
        zpMaterialPartTexture& p = textures.pushBackEmpty();
        p.name = name;
        p.texture = texture;
    }

    void addMatrix( const zp_char* name, const zpMatrix4fData& matrix )
    {
        zpMaterialPartMatrix& p = matrices.pushBackEmpty();
        p.name = name;
        p.matrix = matrix;
    }
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
