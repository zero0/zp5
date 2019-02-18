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

//
//
//

struct zpShaderProperty
{
    ZP_FORCE_INLINE zpShaderProperty()
        : m_property( 0 )
    {
    }

    ZP_FORCE_INLINE zpShaderProperty( const zp_char* name )
        : m_property( zp_hash_fnv_t_str( name, zp_strlen( name ) ) )
    {
        if( !s_shaderPropertyToName->containsKey( m_property ) )
        {
            s_shaderPropertyToName->set( m_property, zpString( name ) );
        }
    }

    ZP_FORCE_INLINE zpShaderProperty( const zpString& name )
        : m_property( zp_hash_fnv_t_str( name.str(), name.length() ) )
    {
        if( !s_shaderPropertyToName->containsKey( m_property ) )
        {
            s_shaderPropertyToName->set( m_property, name );
        }
    }

    ZP_FORCE_INLINE zpShaderProperty( const zpShaderProperty& other )
        : m_property( other.m_property )
    {
    }

    ZP_FORCE_INLINE zpShaderProperty( zpShaderProperty&& other )
        : m_property( other.m_property )
    {
    }

    ZP_FORCE_INLINE void operator=( const zpShaderProperty& other )
    {
        m_property = other.m_property;
    }

    ZP_FORCE_INLINE void operator=( zpShaderProperty&& other )
    {
        m_property = other.m_property;
    }

    ZP_FORCE_INLINE operator zp_hash_t() const
    {
        return m_property;
    }

    ZP_FORCE_INLINE zp_bool isValid() const
    {
        return m_property != 0;
    }

    const zp_char* name() const
    {
        zpString* name;
        return s_shaderPropertyToName->get( m_property, name ) ? name->str() : ZP_NULL;
    }

    friend zp_bool operator==( const zpShaderProperty& x, const zpShaderProperty& y );
    friend zp_bool operator!=( const zpShaderProperty& x, const zpShaderProperty& y );

private:
    ZP_FORCE_INLINE zpShaderProperty( zp_hash_t hash )
        : m_property( hash )
    {
    }

    zp_hash_t m_property;
    static zpMap<zp_hash_t, zpString>* s_shaderPropertyToName;

    friend class zpShaderManager;
};

//
//
//

struct zpShaderKeyword
{
    ZP_FORCE_INLINE zpShaderKeyword()
        : m_keyword( 0 )
    {
    }

    ZP_FORCE_INLINE zpShaderKeyword( const zp_char* keyword )
        : m_keyword( 0 )
    {
        zp_hash_t hash = zp_hash_fnv_t_str( keyword, zp_strlen( keyword ) );
        if( s_shaderKeywordToName->containsKey( hash ) )
        {
            m_keyword = hash;
        }
    }

    ZP_FORCE_INLINE zpShaderKeyword( const zpString& keyword )
        : m_keyword( 0 )
    {
        zp_hash_t hash = zp_hash_fnv_t_str( keyword.str(), keyword.length() );
        if( s_shaderKeywordToName->containsKey( hash ) )
        {
            m_keyword = hash;
        }
    }

    ZP_FORCE_INLINE zpShaderKeyword( const zpShaderKeyword& other )
        : m_keyword( other.m_keyword )
    {
    }

    ZP_FORCE_INLINE zpShaderKeyword( zpShaderKeyword&& other )
        : m_keyword( other.m_keyword )
    {
    }

    ZP_FORCE_INLINE void operator=( const zpShaderKeyword& other )
    {
        m_keyword = other.m_keyword;
    }

    ZP_FORCE_INLINE void operator=( zpShaderKeyword&& other )
    {
        m_keyword = other.m_keyword;
    }


    ZP_FORCE_INLINE operator zp_hash_t() const
    {
        return m_keyword;
    }

    ZP_FORCE_INLINE zp_bool isValid() const
    {
        return m_keyword != 0;
    }

    const zp_char* name() const
    {
        zpString* name;
        return s_shaderKeywordToName->get( m_keyword, name ) ? name->str() : ZP_NULL;
    }

    friend zp_bool operator==( const zpShaderKeyword& x, const zpShaderKeyword& y );
    friend zp_bool operator!=( const zpShaderKeyword& x, const zpShaderKeyword& y );
    friend zp_bool operator<( const zpShaderKeyword& x, const zpShaderKeyword& y );

private:
    ZP_FORCE_INLINE zpShaderKeyword( zp_hash_t hash )
        : m_keyword( hash )
    {
    }

    zp_hash_t m_keyword;
    static zpMap<zp_hash_t, zpString>* s_shaderKeywordToName;

    friend class zpShaderManager;
};

//
//
//

struct zpShaderKeywordSet
{
    zpHashSet< zpShaderKeyword > keywords;
};

//
//
//

class zpShaderManager
{
public:
    zpShaderManager();
    ~zpShaderManager();

    void setup( zpRenderingEngine* engine );
    void teardown();

    zp_bool getShader( const zp_char* shaderName, zpShaderHandle& shader );

    void garbageCollect();

    zpShaderProperty getShaderProperty( const zp_char* name ) const;
    zpShaderKeyword getShaderKeyword( const zp_char* name ) const;
    
private:
    zpVector< zpShaderInstance* > m_shaderInstances;

    zp_hash64 m_newShaderInstanceId;
    zpRenderingEngine* m_engine;

    zpMap<zp_hash_t, zpString> m_shaderPropertyToName;
    zpMap<zp_hash_t, zpString> m_shaderKeywordToName;
};

#endif // !ZP_SHADER_H
