#include "Rendering/zpRendering.h"

zpRenderResource::zpRenderResource()
    : m_type( ZP_RENDER_RESOURCE_TYPE_UNKNOWN )
    , m_name()
    , m_id( 0 )
{
}

zpRenderResource::zpRenderResource( const zp_char* name )
    : m_type( ZP_RENDER_RESOURCE_TYPE_UNKNOWN )
    , m_name( name )
    , m_id( 0 )
{
    m_id = m_name;
}

zpRenderResource::~zpRenderResource()
{
}

const zpString& zpRenderResource::name() const
{
    return m_name;
}

zp_hash_t zpRenderResource::id() const
{
    return m_id;
}

zpRenderResourceType zpRenderResource::type() const
{
    return m_type;
}

void zpRenderResource::setInt( zp_int value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_INT;
    m_int = value;
}

void zpRenderResource::setFloat( zp_float value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_FLOAT;
    m_float = value;
}


void zpRenderResource::setVector2( const zpVector2f& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_VECTOR2;
    m_vector2 = value;
}


void zpRenderResource::setVector4( const zpVector4f& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_VECTOR4;
    m_vector4 = value;
}


void zpRenderResource::setMatrix( const zpMatrix4f& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_MATRIX;
    m_matrix = value;
}

void zpRenderResource::setColor( const zpColorf& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_COLOR;
    m_color = value;
}

void zpRenderResource::setTexture( const zpRenderHandle& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_TEXTURE;
    m_texture = value;
}

zp_int zpRenderResource::getInt() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_INT, "" );
    return m_int;
}

zp_float zpRenderResource::getFloat() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_FLOAT, "" );
    return m_float;
}

const zpVector2f& zpRenderResource::getVector2() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_VECTOR2, "" );
    return m_vector2;
}

const zpVector4f& zpRenderResource::getVector4() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_VECTOR4, "" );
    return m_vector4;
}

const zpMatrix4f& zpRenderResource::getMatrix() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_MATRIX, "" );
    return m_matrix;
}

const zpColorf& zpRenderResource::getColor() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_COLOR, "" );
    return m_color;
}

const zpRenderHandle& zpRenderResource::getTexture() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_TEXTURE, "" );
    return m_texture;
}

zp_int zpRenderResource::asInt() const
{
    zp_int value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_INT:
            value = m_int;
            break;
        case ZP_RENDER_RESOURCE_TYPE_FLOAT:
            value = static_cast<zp_int>( m_float );
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR2:
            value = static_cast<zp_int>( m_vector2.x );
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR4:
            value = static_cast<zp_int>( m_vector4.x );
            break;
        case ZP_RENDER_RESOURCE_TYPE_MATRIX:
            value = static_cast<zp_int>( m_matrix.m[0] );
            break;
        case ZP_RENDER_RESOURCE_TYPE_COLOR:
            value = static_cast<zp_int>( m_color.r );
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

zp_float zpRenderResource::asFloat() const
{
    zp_float value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_INT:
            value = static_cast<zp_float>( m_int );
            break;
        case ZP_RENDER_RESOURCE_TYPE_FLOAT:
            value = m_float;
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR2:
            value = m_vector2.x;
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR4:
            value = m_vector4.x;
            break;
        case ZP_RENDER_RESOURCE_TYPE_MATRIX:
            value = m_matrix.m[ 0 ];
            break;
        case ZP_RENDER_RESOURCE_TYPE_COLOR:
            value = m_color.r;
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

zpVector2f zpRenderResource::asVector2() const
{
    zpVector2f value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_INT:
            value = { static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ) };
            break;
        case ZP_RENDER_RESOURCE_TYPE_FLOAT:
            value = { m_float, m_float };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR2:
            value = m_vector2;
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR4:
            value = { m_vector4.x, m_vector4.y };
            break;
        case ZP_RENDER_RESOURCE_TYPE_MATRIX:
            value = { m_matrix.m[ 0 ], m_matrix.m[ 1 ] };
            break;
        case ZP_RENDER_RESOURCE_TYPE_COLOR:
            value = { m_color.r, m_color.g };
            break;
        default:
            value = { 0, 0 };
            break;
    }
    return value;
}

zpVector4f zpRenderResource::asVector4() const
{
    zpVector4f value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_INT:
            value = { static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ) };
            break;
        case ZP_RENDER_RESOURCE_TYPE_FLOAT:
            value = { m_float, m_float, m_float, m_float };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR2:
            value = { m_vector2.x, m_vector2.y, 0, 0 };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR4:
            value = m_vector4;
            break;
        case ZP_RENDER_RESOURCE_TYPE_MATRIX:
            value = { m_matrix.m[ 0 ], m_matrix.m[ 1 ], m_matrix.m[ 2 ], m_matrix.m[ 3 ] };
            break;
        case ZP_RENDER_RESOURCE_TYPE_COLOR:
            value = { m_color.r, m_color.g, m_color.b, m_color.a };
            break;
        default:
            value = { 0, 0, 0, 0 };
            break;
    }
    return value;
}

zpMatrix4f zpRenderResource::asMatrix() const
{
    zpMatrix4f value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_INT:
            value = {
                    static_cast<zp_float>( m_int ), 0, 0, 0,
                    0, static_cast<zp_float>( m_int ), 0, 0,
                    0, 0, static_cast<zp_float>( m_int ), 0,
                    0, 0, 0, 1 };
            break;
        case ZP_RENDER_RESOURCE_TYPE_FLOAT:
            value = {
                    m_float, 0, 0, 0,
                    0, m_float, 0, 0,
                    0, 0, m_float, 0,
                    0, 0, 0, 1 };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR2:
            value = {
                    m_vector2.x, 0, 0, 0,
                    0, m_vector2.y, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 1 };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR4:
            value = {
                    m_vector4.x, 0, 0, 0,
                    0, m_vector4.y, 0, 0,
                    0, 0, m_vector4.z, 0,
                    0, 0, 0, m_vector4.w };
            break;
        case ZP_RENDER_RESOURCE_TYPE_MATRIX:
            value = m_matrix;
            break;
        case ZP_RENDER_RESOURCE_TYPE_COLOR:
            value = {
                    m_color.r, 0, 0, 0,
                    0, m_color.g, 0, 0,
                    0, 0, m_color.b, 0,
                    0, 0, 0, m_color.a };
            break;
        default:
            value = {};
            break;
    }
    return value;
}

zpColorf zpRenderResource::asColor() const
{
    zpColorf value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_INT:
            value = { static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ), static_cast<zp_float>( m_int ) };
            break;
        case ZP_RENDER_RESOURCE_TYPE_FLOAT:
            value = { m_float, m_float, m_float, m_float };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR2:
            value = { m_vector2.x, m_vector2.y, 0, 0 };
            break;
        case ZP_RENDER_RESOURCE_TYPE_VECTOR4:
            value = { m_vector4.x, m_vector4.y, m_vector4.z, m_vector4.w };
            break;
        case ZP_RENDER_RESOURCE_TYPE_MATRIX:
            value = { m_matrix.m[ 0 ], m_matrix.m[ 1 ], m_matrix.m[ 2 ], m_matrix.m[ 3 ] };
            break;
        case ZP_RENDER_RESOURCE_TYPE_COLOR:
            value = m_color;
            break;
        default:
            value = { 0, 0, 0, 0 };
            break;
    }
    return value;
}

zpRenderHandle zpRenderResource::asTexture() const
{
    zpRenderHandle value;
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_TEXTURE:
            value = m_texture;
            break;
        default:
            value = {};
            break;
    }
    return value;
}

void zpRenderResource::reset()
{

}
