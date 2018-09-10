#pragma once
#ifndef ZP_SHADER_H
#define ZP_SHADER_H

struct zpShader
{
    zpRenderHandle programShader;
    zpRenderHandle vertexShader;
    zpRenderHandle fragmentShader;
    zpRenderHandle geometryShader;
};

struct zpShaderInstance
{
    zpString shaderName;
    zp_size_t refCount;
    zp_hash64 instanceId;
    zp_time_t lastModifiedTime;
    zpShader shader;
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
    zpVector< zpShaderInstance* > m_shaderInstances;

    zp_hash64 m_newShaderInstanceId;
    zpRenderingEngine* m_engine;

    zpMap<zp_hash_t, zpString> m_shaderPropertyToName;
};

struct zpShaderProperty
{
    ZP_FORCE_INLINE zpShaderProperty()
        : property( 0 )
    {
    }

    ZP_FORCE_INLINE zpShaderProperty( const zp_char* name )
        : property( zp_hash_fnv64_str( name, zp_strlen( name ) ) )
    {
        if( !s_shaderPropertyToName->containsKey( property ) )
        {
            s_shaderPropertyToName->set( property, zpString( name ) );
        }
    }

    ZP_FORCE_INLINE zpShaderProperty( const zpString& name )
        : property( zp_hash_fnv64_str( name.str(), name.length() ) )
    {
        if( !s_shaderPropertyToName->containsKey( property ) )
        {
            s_shaderPropertyToName->set( property, name );
        }
    }

    ZP_FORCE_INLINE zpShaderProperty( const zpShaderProperty& other )
        : property( other.property )
    {
    }

    ZP_FORCE_INLINE zpShaderProperty( zpShaderProperty&& other )
        : property( other.property )
    {
    }

    ZP_FORCE_INLINE void operator=( const zpShaderProperty& other )
    {
        property = other.property;
    }

    ZP_FORCE_INLINE void operator=( zpShaderProperty&& other )
    {
        property = other.property;
    }

    ZP_FORCE_INLINE operator zp_hash_t() const
    {
        return property;
    }

    const zp_char* name() const
    {
        zpString* name;
        return s_shaderPropertyToName->get( property, name ) ? name->str() : ZP_NULL;
    }

    zp_hash_t property;

private:
    static zpMap<zp_hash_t, zpString>* s_shaderPropertyToName;

    friend class zpShaderManager;
};

static ZP_FORCE_INLINE zp_bool operator==( const zpShaderProperty& x, const zpShaderProperty& y )
{
    return x.property == y.property;
}

static ZP_FORCE_INLINE zp_bool operator!=( const zpShaderProperty& x, const zpShaderProperty& y )
{
    return x.property != y.property;
}

#endif // !ZP_SHADER_H
