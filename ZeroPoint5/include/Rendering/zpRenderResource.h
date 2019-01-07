#pragma once
#ifndef ZP_RENDER_RESOURCE_H
#define ZP_RENDER_RESOURCE_H

enum zpRenderResourceType : zp_uint
{
    ZP_RENDER_RESOURCE_TYPE_UNKNOWN,
    ZP_RENDER_RESOURCE_TYPE_INT,
    ZP_RENDER_RESOURCE_TYPE_FLOAT,
    ZP_RENDER_RESOURCE_TYPE_VECTOR2,
    ZP_RENDER_RESOURCE_TYPE_VECTOR4,
    ZP_RENDER_RESOURCE_TYPE_MATRIX,
    ZP_RENDER_RESOURCE_TYPE_COLOR,
    ZP_RENDER_RESOURCE_TYPE_TEXTURE,
    ZP_RENDER_RESOURCE_TYPE_RENDER_BUFFER,
};

class zpRenderResource
{
public:
    explicit zpRenderResource( const zp_char* name, zpRenderingEngine* engine );
    ~zpRenderResource();

    const zpString& name() const;
    zp_hash_t id() const;
    zpRenderResourceType type() const;

    void setInt( zp_int value );
    void setFloat( zp_float value );
    void setVector2( const zpVector2f& value );
    void setVector4( const zpVector4f& value );
    void setMatrix( const zpMatrix4f& value );
    void setColor( const zpColorf& value );
    void setTexture( const zpTexture& value );
    void setBuffer( const zpRenderBuffer& value );

    zp_int getInt() const;
    zp_float getFloat() const;
    const zpVector2f& getVector2() const;
    const zpVector4f& getVector4() const;
    const zpMatrix4f& getMatrix() const;
    const zpColorf& getColor() const;
    const zpTexture& getTexture() const;
    const zpRenderBuffer& getBuffer() const;

    zp_int asInt() const;
    zp_float asFloat() const;
    zpVector2f asVector2() const;
    zpVector4f asVector4() const;
    zpMatrix4f asMatrix() const;
    zpColorf asColor() const;
    zpTexture asTexture() const;

private:
    zpRenderResource();

    void reset();

    zpRenderResourceType m_type;
    zp_hash_t m_id;
    zpRenderingEngine* m_engine;
    zpString m_name;

    union
    {
        zp_int m_int;
        zp_float m_float;
        zpVector2f m_vector2;
        zpVector4f m_vector4;
        zpMatrix4f m_matrix;
        zpColorf m_color;
        zpTexture m_texture;
        zpRenderBuffer m_buffer;
    };
};

//
//
//

class zpRenderResourceCache;

class zpRenderResourceIdentifier
{
public:
    zpRenderResourceIdentifier();
    ~zpRenderResourceIdentifier();

    zpRenderResourceType type() const;
    const zpString& name() const;

    zp_int getInt() const;
    zp_float getFloat() const;
    const zpVector2f& getVector2() const;
    const zpVector4f& getVector4() const;
    const zpColorf& getColor() const;
    const zpMatrix4f& getMatrix() const;
    const zpTexture& getTexture() const;
    const zpRenderBuffer& getRenderBuffer() const;

    void setInt( zp_int value );
    void setFloat( zp_float value );
    void setVector2( const zpVector2f& value );
    void setVector4( const zpVector4f& value );
    void setColor( const zpColorf& value );
    void setMatrix( const zpMatrix4f& value );
    void setTexture( const zpTexture& value );
    void setBuffer( const zpRenderBuffer& value );

private:
    zp_size_t m_dataIndex;
    zp_size_t m_nameIndex;
    zpRenderResourceCache* m_cache;
    zpRenderResourceType m_type;

    friend zpRenderResourceCache;
};

//
//
//

class zpRenderResourceCache
{
public:
    void setup( zpRenderingEngine* engine );
    void teardown();

    zp_bool isRenderResource( const zp_char* name ) const;

    zp_bool tryGetRenderResource( const zp_char* name, zpRenderResourceIdentifier& outResource );

    zp_bool createInt( const zp_char* name, zp_int value, zpRenderResourceIdentifier& outResource );
    zp_bool createFloat( const zp_char* name, zp_float value, zpRenderResourceIdentifier& outResource );
    zp_bool createVector2( const zp_char* name, const zpVector2f& value, zpRenderResourceIdentifier& outResource );
    zp_bool createVector4( const zp_char* name, const zpVector4f& value, zpRenderResourceIdentifier& outResource );
    zp_bool createColor( const zp_char* name, const zpColorf& value, zpRenderResourceIdentifier& outResource );
    zp_bool createMatrix( const zp_char* name, const zpMatrix4f& value, zpRenderResourceIdentifier& outResource );
    zp_bool createTexture( const zp_char* name, const zpTexture& value, zpRenderResourceIdentifier& outResource );
    zp_bool createRenderBuffer( const zp_char* name, const zpRenderBuffer& value, zpRenderResourceIdentifier& outResource );

private:
    const zpString& getName( zp_size_t index ) const;

    zp_int getInt( zp_size_t index ) const;
    zp_float getFloat( zp_size_t index ) const;
    const zpVector2f& getVector2( zp_size_t index ) const;
    const zpVector4f& getVector4( zp_size_t index ) const;
    const zpColorf& getColor( zp_size_t index ) const;
    const zpMatrix4f& getMatrix( zp_size_t index ) const;
    const zpTexture& getTexture( zp_size_t index ) const;
    const zpRenderBuffer& getRenderBuffer( zp_size_t index ) const;

    void setInt( zp_size_t index, zp_int value );
    void setFloat( zp_size_t index, zp_float value );
    void setVector2( zp_size_t index, const zpVector2f& value );
    void setVector4( zp_size_t index, const zpVector4f& value );
    void setColor( zp_size_t index, const zpColorf& value );
    void setMatrix( zp_size_t index, const zpMatrix4f& value );
    void setTexture( zp_size_t index, const zpTexture& value );
    void setRenderBuffer( zp_size_t index, const zpRenderBuffer& value );

private:
    struct zpRenderResourceInstance
    {
        zp_size_t nameIndex;
        zp_size_t dataIndex;
        zpRenderResourceType type;
    };

    zpRenderingEngine* m_engine;

    zpMap< zp_hash_t, zpRenderResourceInstance > m_instances;

    zpVector< zpString > m_names;

    zpVector< zp_int > m_ints;
    zpVector< zp_float > m_floats;
    zpVector< zpVector2f > m_vector2;
    zpVector< zpVector4f > m_vector4;
    zpVector< zpColorf > m_colors;
    zpVector< zpMatrix4f > m_matricies;
    zpVector< zpTexture > m_textures;
    zpVector< zpRenderBuffer > m_renderBuffers;

    friend zpRenderResourceIdentifier;
};

#endif // ZP_RENDER_RESOURCE_H
