#pragma once
#ifndef ZP_MATERIAL_H
#define ZP_MATERIAL_H

struct zpMaterialPartFloat
{
    zpString name;
    float value;
};

struct zpMaterialPartColor
{
    zpString name;
    zpColorf color;
};

struct zpMaterialPartVector
{
    zpString name;
    zpVector4f vector;
};

struct zpMaterialPartTexture
{
    zpString name;
    zpTextureHandle texture;
};

struct zpMaterialPartMatrix
{
    zpString name;
    zpMatrix4f matrix;
};

struct zpMaterialOld
{
    zpColorf color;

    zpVector4f mainTexST;
    zpVector4f specTexST;
    zpVector4f normTexST;

    zpShaderHandle shader;
    zpTextureHandle mainTex;
    zpTextureHandle specTex;
    zpTextureHandle normTex;
};

struct zpMaterial
{
    zp_uint renderQueue;
    zpShaderHandle shader;

    zpMap< zpShaderProperty, zp_int > ints;
    zpMap< zpShaderProperty, zp_float > floats;
    zpMap< zpShaderProperty, zpColorf > colors;
    zpMap< zpShaderProperty, zpVector4f > vectors;
    zpMap< zpShaderProperty, zpTextureHandle > textures;
    zpMap< zpShaderProperty, zpMatrix4f > matrices;

    void setInt( zpShaderProperty name, const zp_int value )
    {
        ints.set( name, value );
    }

    void setFloat( zpShaderProperty name, const zp_float value )
    {
        floats.set( name, value );
    }

    void setColor( zpShaderProperty name, const zpColorf& color )
    {
        colors.set( name, color );
    }

    void setVector( zpShaderProperty name, const zpVector4f& vector )
    {
        vectors.set( name, vector );
    }

    void setTexture( zpShaderProperty name, const zpTextureHandle& texture )
    {
        textures.set( name, texture );
    }

    void setMatrix( zpShaderProperty name, const zpMatrix4f& matrix )
    {
        matrices.set( name, matrix );
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

    ZP_FORCE_INLINE const zpMaterial* operator->() const
    {
        return get();
    }
    ZP_FORCE_INLINE zpMaterial* operator->()
    {
        return get();
    }

    const zpMaterial* get() const;
    zpMaterial* get();


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
