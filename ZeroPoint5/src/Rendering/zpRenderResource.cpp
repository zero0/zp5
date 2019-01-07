#include "Rendering/zpRendering.h"

zpRenderResource::zpRenderResource()
    : m_type( ZP_RENDER_RESOURCE_TYPE_UNKNOWN )
    , m_name()
    , m_id( 0 )
    , m_engine( ZP_NULL )
{
}

zpRenderResource::zpRenderResource( const zp_char* name, zpRenderingEngine* engine )
    : m_type( ZP_RENDER_RESOURCE_TYPE_UNKNOWN )
    , m_name( name )
    , m_id( 0 )
    , m_engine( engine )
{
    m_id = m_name;
}

zpRenderResource::~zpRenderResource()
{
    reset();
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

void zpRenderResource::setTexture( const zpTexture& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_TEXTURE;
    m_texture = value;
}

void zpRenderResource::setBuffer( const zpRenderBuffer& value )
{
    reset();

    m_type = ZP_RENDER_RESOURCE_TYPE_RENDER_BUFFER;
    m_buffer = value;
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

const zpTexture& zpRenderResource::getTexture() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_TEXTURE, "" );
    return m_texture;
}

const zpRenderBuffer& zpRenderResource::getBuffer() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_RENDER_BUFFER, "" );
    return m_buffer;
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

zpTexture zpRenderResource::asTexture() const
{
    zpTexture value;
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
    switch( m_type )
    {
        case ZP_RENDER_RESOURCE_TYPE_TEXTURE:
            m_engine->destroyTexture( m_texture );
            break;
    }

    m_type = ZP_RENDER_RESOURCE_TYPE_UNKNOWN;
}

//
//
//

zpRenderResourceIdentifier::zpRenderResourceIdentifier()
    : m_dataIndex( 0 )
    , m_nameIndex( 0 )
    , m_cache( ZP_NULL )
    , m_type( ZP_RENDER_RESOURCE_TYPE_UNKNOWN )
{
}

zpRenderResourceIdentifier::~zpRenderResourceIdentifier()
{
}

zpRenderResourceType zpRenderResourceIdentifier::type() const
{
    return m_type;
}

const zpString& zpRenderResourceIdentifier::name() const
{
    return m_cache->getName( m_nameIndex );
}

zp_int zpRenderResourceIdentifier::getInt() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_INT, "" );
    return m_cache->getInt( m_dataIndex );
}

zp_float zpRenderResourceIdentifier::getFloat() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_FLOAT, "" );
    return m_cache->getFloat( m_dataIndex );
}

const zpVector2f& zpRenderResourceIdentifier::getVector2() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_VECTOR2, "" );
    return m_cache->getVector2( m_dataIndex );
}

const zpVector4f& zpRenderResourceIdentifier::getVector4() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_VECTOR4, "" );
    return m_cache->getVector4( m_dataIndex );
}

const zpMatrix4f& zpRenderResourceIdentifier::getMatrix() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_MATRIX, "" );
    return m_cache->getMatrix( m_dataIndex );
}

const zpColorf& zpRenderResourceIdentifier::getColor() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_COLOR, "" );
    return m_cache->getColor( m_dataIndex );
}

const zpTexture& zpRenderResourceIdentifier::getTexture() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_TEXTURE, "" );
    return m_cache->getTexture( m_dataIndex );
}

const zpRenderBuffer& zpRenderResourceIdentifier::getRenderBuffer() const
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_RENDER_BUFFER, "" );
    return m_cache->getRenderBuffer( m_dataIndex );
}

void zpRenderResourceIdentifier::setInt( zp_int value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_INT, "" );
    m_cache->setInt( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setFloat( zp_float value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_FLOAT, "" );
    m_cache->setFloat( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setVector2( const zpVector2f& value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_VECTOR2, "" );
    m_cache->setVector2( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setVector4( const zpVector4f& value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_VECTOR4, "" );
    m_cache->setVector4( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setMatrix( const zpMatrix4f& value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_MATRIX, "" );
    m_cache->setMatrix( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setColor( const zpColorf& value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_COLOR, "" );
    m_cache->setColor( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setTexture( const zpTexture& value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_TEXTURE, "" );
    m_cache->setTexture( m_dataIndex, value );
}

void zpRenderResourceIdentifier::setBuffer( const zpRenderBuffer& value )
{
    ZP_ASSERT( m_type == ZP_RENDER_RESOURCE_TYPE_RENDER_BUFFER, "" );
    m_cache->setRenderBuffer( m_dataIndex, value );
}

//
//
//

void zpRenderResourceCache::setup( zpRenderingEngine* engine )
{
    m_engine = engine;
}

void zpRenderResourceCache::teardown()
{
    m_ints.clear();
    m_floats.clear();
    m_vector2.clear();
    m_vector4.clear();
    m_colors.clear();
    m_matricies.clear();

    zpVector< zpTexture >::iterator tb = m_textures.begin();
    zpVector< zpTexture >::iterator te = m_textures.end();
    for( ; tb != te; ++tb )
    {
        m_engine->destroyTexture( *tb );
    }
    m_textures.clear();

    zpVector< zpRenderBuffer >::iterator rb = m_renderBuffers.begin();
    zpVector< zpRenderBuffer >::iterator re = m_renderBuffers.end();
    for( ; rb != re; ++rb )
    {
        m_engine->destoryRenderBuffer( *rb );
    }
    m_renderBuffers.clear();

    m_engine = ZP_NULL;
}

zp_bool zpRenderResourceCache::isRenderResource( const zp_char* name ) const
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    return m_instances.containsKey( hash );
}

zp_bool zpRenderResourceCache::tryGetRenderResource( const zp_char* name, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );

    zpRenderResourceInstance* inst;
    zp_bool found = m_instances.get( hash, inst );
    if( found )
    {
        outResource.m_dataIndex = inst->dataIndex;
        outResource.m_nameIndex = inst->nameIndex;
        outResource.m_type = inst->type;
        outResource.m_cache = this;
    }

    return found;
}
zp_bool zpRenderResourceCache::createInt( const zp_char* name, zp_int value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_ints.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_INT;

        m_instances.set( hash, inst );

        m_ints.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createFloat( const zp_char* name, zp_float value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_floats.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_FLOAT;

        m_instances.set( hash, inst );

        m_floats.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createVector2( const zp_char* name, const zpVector2f& value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_vector2.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_VECTOR2;

        m_instances.set( hash, inst );

        m_vector2.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createVector4( const zp_char* name, const zpVector4f& value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_vector4.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_VECTOR4;

        m_instances.set( hash, inst );

        m_vector4.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createColor( const zp_char* name, const zpColorf& value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_colors.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_COLOR;

        m_instances.set( hash, inst );

        m_colors.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createMatrix( const zp_char* name, const zpMatrix4f& value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_matricies.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_MATRIX;

        m_instances.set( hash, inst );

        m_matricies.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createTexture( const zp_char* name, const zpTexture& value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_textures.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_TEXTURE;

        m_instances.set( hash, inst );

        m_textures.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

zp_bool zpRenderResourceCache::createRenderBuffer( const zp_char* name, const zpRenderBuffer& value, zpRenderResourceIdentifier& outResource )
{
    zp_hash_t hash = zp_hash_fnv_t_str( name, zp_strlen( name ) );
    zp_bool found = m_instances.containsKey( hash );
    if( !found )
    {
        zpRenderResourceInstance inst;
        inst.dataIndex = m_renderBuffers.size();
        inst.nameIndex = m_names.size();
        inst.type = ZP_RENDER_RESOURCE_TYPE_RENDER_BUFFER;

        m_instances.set( hash, inst );

        m_renderBuffers.pushBack( value );
        m_names.pushBack( zpString( name ) );

        outResource.m_dataIndex = inst.dataIndex;
        outResource.m_nameIndex = inst.nameIndex;
        outResource.m_type = inst.type;
        outResource.m_cache = this;
    }

    return !found;
}

const zpString& zpRenderResourceCache::getName( zp_size_t index ) const
{
    return m_names[ index ];
}

zp_int zpRenderResourceCache::getInt( zp_size_t index ) const
{
    return m_ints[ index ];
}

zp_float zpRenderResourceCache::getFloat( zp_size_t index ) const
{
    return m_floats[ index ];
}

const zpVector2f& zpRenderResourceCache::getVector2( zp_size_t index ) const
{
    return m_vector2[ index ];
}

const zpVector4f& zpRenderResourceCache::getVector4( zp_size_t index ) const
{
    return m_vector4[ index ];
}

const zpColorf& zpRenderResourceCache::getColor( zp_size_t index ) const
{
    return m_colors[ index ];
}

const zpMatrix4f& zpRenderResourceCache::getMatrix( zp_size_t index ) const
{
    return m_matricies[ index ];
}

const zpTexture& zpRenderResourceCache::getTexture( zp_size_t index ) const
{
    return m_textures[ index ];
}

const zpRenderBuffer& zpRenderResourceCache::getRenderBuffer( zp_size_t index ) const
{
    return m_renderBuffers[ index ];
}

void zpRenderResourceCache::setInt( zp_size_t index, zp_int value )
{
    m_ints[ index ] = value;
}

void zpRenderResourceCache::setFloat( zp_size_t index, zp_float value )
{
    m_floats[ index ] = value;
}

void zpRenderResourceCache::setVector2( zp_size_t index, const zpVector2f& value )
{
    m_vector2[ index ] = value;
}

void zpRenderResourceCache::setVector4( zp_size_t index, const zpVector4f& value )
{
    m_vector4[ index ] = value;
}

void zpRenderResourceCache::setColor( zp_size_t index, const zpColorf& value )
{
    m_colors[ index ] = value;
}

void zpRenderResourceCache::setMatrix( zp_size_t index, const zpMatrix4f& value )
{
    m_matricies[ index ] = value;
}

void zpRenderResourceCache::setTexture( zp_size_t index, const zpTexture& value )
{
    m_textures[ index ] = value;
}

void zpRenderResourceCache::setRenderBuffer( zp_size_t index, const zpRenderBuffer& value )
{
    m_renderBuffers[ index ] = value;
}
