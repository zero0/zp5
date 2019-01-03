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
    ZP_RENDER_RESOURCE_TYPE_RENDER_TEXTURE,
    ZP_RENDER_RESOURCE_TYPE_BUFFER,
};

class zpRenderResource
{
public:
    explicit zpRenderResource( const zp_char* name );
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
    void setTexture( const zpRenderHandle& value );

    zp_int getInt() const;
    zp_float getFloat() const;
    const zpVector2f& getVector2() const;
    const zpVector4f& getVector4() const;
    const zpMatrix4f& getMatrix() const;
    const zpColorf& getColor() const;
    const zpRenderHandle& getTexture() const;

    zp_int asInt() const;
    zp_float asFloat() const;
    zpVector2f asVector2() const;
    zpVector4f asVector4() const;
    zpMatrix4f asMatrix() const;
    zpColorf asColor() const;
    zpRenderHandle asTexture() const;

private:
    zpRenderResource();

    void reset();

    zpRenderResourceType m_type;
    zp_hash_t m_id;
    zpString m_name;
    union
    {
        zp_int m_int;
        zp_float m_float;
        zpVector2f m_vector2;
        zpVector4f m_vector4;
        zpMatrix4f m_matrix;
        zpColorf m_color;
        zpRenderHandle m_texture;
    };
};

#endif // ZP_RENDER_RESOURCE_H
