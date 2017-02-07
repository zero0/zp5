#include "zpCore.h"
#include <math.h>

zp_float zp_cos( zp_float r )
{
    return cosf( r );
}
zp_float zp_sin( zp_float r )
{
    return sinf( r );
}
zp_float zp_tan( zp_float r )
{
    return tanf( r );
}

void zpMath::PlaneSet( zpPlane& plane, zp_float a, zp_float b, zp_float c, zp_float d )
{
    plane.normal = { a, b, c, 0 };
    plane.d = d;
}

void zpMath::PlaneSet( zpPlane& plane, zpVector4fParamF p0, zpVector4fParamF p1, zpVector4fParamF p2 )
{
    zpVector4f p;
    zpVector4f p01, p02;
    zpScalar d;

    p01 = zpMath::Vector4Sub( p0, p1 );
    p02 = zpMath::Vector4Sub( p2, p1 );
    p = zpMath::Vector4Cross3( p01, p02 );
    p = zpMath::Vector4Normalize3( p );

    d = zpMath::Vector4Dot3( p, p1 );
    d = zpMath::Vector4Neg( d );

    ZP_ALIGN16 zpVector4fData n;
    zpMath::Vector4Store4( p, n.m );

    plane.normal = n;
    plane.d = zpMath::AsFloat( d );
}

void zpMath::PlaneSet( zpPlane& plane, const zpVector4fData& p0, const zpVector4fData& p1, const zpVector4fData& p2 )
{
    zpVector4f p;
    zpVector4f a0, a1, a2;
    zpVector4f p01, p02;
    zpScalar d;

    a0 = zpMath::Vector4Load4( p0.m );
    a1 = zpMath::Vector4Load4( p1.m );
    a2 = zpMath::Vector4Load4( p2.m );

    p01 = zpMath::Vector4Sub( a0, a1 );
    p02 = zpMath::Vector4Sub( a2, a1 );
    p = zpMath::Vector4Cross3( p01, p02 );
    p = zpMath::Vector4Normalize3( p );

    d = zpMath::Vector4Dot3( p, a1 );
    d = zpMath::Vector4Neg( d );

    ZP_ALIGN16 zpVector4fData n;
    zpMath::Vector4Store4( p, n.m );

    plane.normal = n;
    plane.d = zpMath::AsFloat( d );
}

zp_int zpMath::PlaneGetSide( const zpPlane& plane, const zpVector4fData& p )
{
    zpVector4f n, pos;
    n = zpMath::Vector4Load4( plane.normal.m );
    pos = zpMath::Vector4Load4( p.m );

    zpScalar z = zpMath::Scalar( 0.0f );
    zpScalar d;
    d = zpMath::Vector4Dot3( pos, n );
    d = zpMath::Vector4Add( d, zpMath::Scalar( plane.d ) );

    zp_int c;
    c = zpMath::ScalarCmp( d, z );

    return c;
}

zp_float zpMath::PlaneDistanceToPoint( const zpPlane& plane, const zpVector4fData& p )
{
    zpVector4f n, pos;
    n = zpMath::Vector4Load4( plane.normal.m );
    pos = zpMath::Vector4Load4( p.m );

    zpScalar d;
    d = zpMath::Vector4Dot3( pos, n );
    d = zpMath::Vector4Add( d, zpMath::Scalar( plane.d ) );

    return zpMath::AsFloat( d );
}

void zpMath::FrustrumSetLookTo( zpFrustum& frustrum, const zpVector4fData& eye, const zpVector4fData& direction, const zpVector4fData& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar )
{
    zpVector4f fc;
    zpVector4f nc;
    zpVector4f dir = zpMath::Vector4Load4( direction.m );
    zpVector4f e = zpMath::Vector4Load4( eye.m );
    zpVector4f u = zpMath::Vector4Load4( up.m );

    zpScalar zNearS = zpMath::Scalar( zNear );
    zpScalar zFarS = zpMath::Scalar( zFar );
    zpScalar r = zpMath::Scalar( ratio );

    zp_float t = zp_tan( ZP_DEG_TO_RAD( fovy ) * 0.5f );
    zpScalar tang = zpMath::Scalar( t );

    zpScalar nh;//( zNear * tang );
    zpScalar nw;//( zNear * tang * ratio );
    zpScalar fh;//( zFar * tang );
    zpScalar fw;//( zFar * tang * ratio );

    nh = zpMath::Vector4Mul( zNearS, tang );
    nw = zpMath::Vector4Mul( nh, r );

    fh = zpMath::Vector4Mul( zFarS, tang );
    fw = zpMath::Vector4Mul( fh, r );

    nc = zpMath::Vector4Mul( e, zpMath::Vector4Scale( dir, zNearS ) );
    fc = zpMath::Vector4Mul( e, zpMath::Vector4Scale( dir, zFarS ) );

    zpVector4f x;
    x = zpMath::Vector4Cross3( u, dir );
    x = zpMath::Vector4Normalize3( x );

    zpVector4f y;
    y = zpMath::Vector4Cross3( dir, x );

    zpVector4f yNh, yFh;
    zpVector4f xNw, xFw;
    yNh = zpMath::Vector4Mul( y, nh );
    xNw = zpMath::Vector4Mul( x, nw );
    yFh = zpMath::Vector4Mul( y, fh );
    xFw = zpMath::Vector4Mul( x, fw );

    // near
    zpVector4f ntl;
    zpVector4f ntr;
    zpVector4f nbl;
    zpVector4f nbr;

    // far
    zpVector4f ftl;
    zpVector4f ftr;
    zpVector4f fbl;
    zpVector4f fbr;

    // near
    ntl = zpMath::Vector4Add( nc, yNh );
    ntl = zpMath::Vector4Sub( ntl, xNw );

    ntr = zpMath::Vector4Add( nc, yNh );
    ntr = zpMath::Vector4Add( ntr, xNw );

    nbl = zpMath::Vector4Sub( nc, yNh );
    nbl = zpMath::Vector4Sub( nbl, xNw );

    nbr = zpMath::Vector4Sub( nc, yNh );
    nbr = zpMath::Vector4Add( nbr, xNw );

    // far
    ftl = zpMath::Vector4Add( fc, yFh );
    ftl = zpMath::Vector4Sub( ftl, xFw );

    ftr = zpMath::Vector4Add( fc, yFh );
    ftr = zpMath::Vector4Add( ftr, xFw );

    fbl = zpMath::Vector4Sub( fc, yFh );
    fbl = zpMath::Vector4Sub( fbl, xFw );

    fbr = zpMath::Vector4Sub( fc, yFh );
    fbr = zpMath::Vector4Add( fbr, xFw );

    //m_top.set(        m_ntl, m_ftl, m_ntr );
    //m_bottom.set(    m_nbr, m_fbr, m_nbl );
    //m_left.set(        m_fbl, m_ftl, m_nbl );
    //m_right.set(    m_ntr, m_ftr, m_nbr );
    //m_near.set(        m_nbl, m_ntl, m_nbr );
    //m_far.set(        m_ftl, m_ftr, m_fbl );

    //m_top.set(    m_ntr, m_ntl, m_ftl );
    //m_bottom.set( m_nbl, m_nbr, m_fbr );
    //m_left.set(   m_ntl, m_nbl, m_fbl );
    //m_right.set(  m_nbr, m_ntr, m_fbr );
    //m_near.set(   m_ntl, m_ntr, m_nbr );
    //m_far.set(    m_ftr, m_ftl, m_fbl );

    zpMath::PlaneSet( frustrum.top,     ntr, ntl, ftl );
    zpMath::PlaneSet( frustrum.bottom,  nbl, nbr, fbr );
    zpMath::PlaneSet( frustrum.left,    ntl, nbl, fbl );
    zpMath::PlaneSet( frustrum.right,   nbr, ntr, fbr );
    zpMath::PlaneSet( frustrum.near,    ntl, ntr, nbr );
    zpMath::PlaneSet( frustrum.far,     ftr, ftl, fbl );
}

//
// AABB
//

zpCollisionResult zpCollision::Test( const zpBoundingAABB& bounds, const zpVector4fData& pos )
{
    zpVector4f c, e, p;
    c = zpMath::Vector4Load4( bounds.center.m );
    e = zpMath::Vector4Load4( bounds.extents.m );
    p = zpMath::Vector4Load4( pos.m );

    zpVector4f pointDiff = zpMath::Vector4Sub( c, p );
    pointDiff = zpMath::Vector4Abs( pointDiff );

    zpVector4fCmp cmp = zpMath::Vector4Cmp( pointDiff, e );

    zp_bool collision = cmp.cmpX != 1 && cmp.cmpY != 1 && cmp.cmpZ != 1;
    return collision ? ZP_COLLISION_RESULT_CONTAINS : ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpBoundingAABB& aabb, const zpRay& ray )
{
    return ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpBoundingAABB& bounds, const zpBoundingAABB& box )
{
    zpVector4f ae, ac, be, bc;
    ac = zpMath::Vector4Load4( bounds.center.m );
    ae = zpMath::Vector4Load4( bounds.extents.m );
    bc = zpMath::Vector4Load4( box.center.m );
    be = zpMath::Vector4Load4( box.extents.m );

    zpVector4f extDiff = zpMath::Vector4Add( ae, be );
    zpVector4f pointDiff = zpMath::Vector4Sub( ac, bc );
    pointDiff = zpMath::Vector4Abs( pointDiff );

    zpVector4fCmp cmp = zpMath::Vector4Cmp( pointDiff, extDiff );

    zp_bool collision = cmp.cmpX != 1 && cmp.cmpY != 1 && cmp.cmpZ != 1;
    return collision ? ZP_COLLISION_RESULT_CONTAINS : ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpBoundingAABB& bounds, const zpBoundingSphere& sphere )
{
    return ZP_COLLISION_RESULT_NONE;
}

//
// Sphere
//

zpCollisionResult zpCollision::Test( const zpBoundingSphere& bounds, const zpVector4fData& pos )
{
    zpVector4f dist, a, b;
    zpScalar d;
    zpScalar r = zpMath::Scalar( bounds.radius );

    b = zpMath::Vector4Load4( pos.m );
    a = zpMath::Vector4Load4( bounds.center.m );

    dist = zpMath::Vector4Sub( b, a );
    d = zpMath::Vector4LengthSquared3( dist );
    r = zpMath::ScalarMul( r, r );

    zp_int c;
    c = zpMath::ScalarCmp( d, r );

    return c == 0 ? ZP_COLLISION_RESULT_INTERSECT : c < 0 ? ZP_COLLISION_RESULT_CONTAINS : ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpBoundingSphere& bounds, const zpRay& ray )
{
    return ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpBoundingSphere& bounds, const zpBoundingAABB& box )
{
    return ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpBoundingSphere& bounds, const zpBoundingSphere& sphere )
{
    zpScalar d, r = zpMath::Scalar( bounds.radius ), g = zpMath::Scalar( sphere.radius );
    zpVector4f dist, a, b;

    a = zpMath::Vector4Load4( bounds.center.m );
    b = zpMath::Vector4Load4( sphere.center.m );
    
    dist = zpMath::Vector4Sub( b, a );
    d = zpMath::Vector4LengthSquared3( dist );

    r = zpMath::ScalarMul( r, r );
    g = zpMath::ScalarMul( g, g );
    r = zpMath::ScalarMul( r, g );

    zp_int c;
    c = zpMath::ScalarCmp( d, r );

    return c == 0 ? ZP_COLLISION_RESULT_INTERSECT : c < 0 ? ZP_COLLISION_RESULT_CONTAINS : ZP_COLLISION_RESULT_NONE;
}

//
// Frustum
//

zpCollisionResult zpCollision::Test( const zpFrustum& a, const zpVector4fData& pos )
{
    zpCollisionResult type = ZP_COLLISION_RESULT_CONTAINS;

    for( zp_uint i = 0; i < 6; ++i )
    {
        zp_int side = zpMath::PlaneGetSide( a.planes[ i ], pos );
        if( side < 0 )
        {
            return ZP_COLLISION_RESULT_NONE;
        }
        else if( side == 0 )
        {
            type = ZP_COLLISION_RESULT_INTERSECT;
            break;
        }
    }

    return type;
}

zpCollisionResult zpCollision::Test( const zpFrustum& a, const zpRay& b )
{
    return ZP_COLLISION_RESULT_NONE;
}

zpCollisionResult zpCollision::Test( const zpFrustum& a, const zpBoundingAABB& b )
{
    zpVector4f center = zpMath::Vector4Load4( b.center.m );
    zpVector4f extence = zpMath::Vector4Load4( b.extents.m );

    zpVector4f absP;
    zpScalar d, r;
    zpScalar nd;
    zpScalar dpr, dmr;
    zp_int cp, cn;

    zpCollisionResult type = ZP_COLLISION_RESULT_CONTAINS;
    for( zp_uint i = 0; i < 6; ++i )
    {
        zpVector4f p = zpMath::Vector4Load4( a.planes[ i ].normal.m );
        absP = zpMath::Vector4Abs( p );

        d = zpMath::Vector4Dot3( center, p );
        r = zpMath::Vector4Dot3( extence, absP );

        nd = zpMath::ScalarNeg( zpMath::Vector4GetW( p ) );

        dpr = zpMath::ScalarAdd( d, r );
        dmr = zpMath::ScalarSub( d, r );

        cp = zpMath::ScalarCmp( dpr, nd );
        cn = zpMath::ScalarCmp( dmr, nd );

        if( cp < 0 )
        {
            type = ZP_COLLISION_RESULT_NONE;
            break;
        }
        else if( cn < 0 )
        {
            type = ZP_COLLISION_RESULT_INTERSECT;
        }
    }

    return type;
}

zpCollisionResult zpCollision::Test( const zpFrustum& a, const zpBoundingSphere& b )
{
    zpScalar z = zpMath::Scalar( 0.0f );
    zpScalar d;
    zp_int c;

    zpVector4f pn, cen = zpMath::Vector4Load4( b.center.m );
    zpScalar r = zpMath::Scalar( b.radius );

    for( zp_uint i = 0; i < 6; ++i )
    {
        pn = zpMath::Vector4Load4( a.planes[ i ].normal.m );

        d = zpMath::Vector4Dot3( pn, cen );
        d = zpMath::ScalarAdd( d, r );
        c = zpMath::ScalarCmp( d, z );

        if( c < 0 )
        {
            return ZP_COLLISION_RESULT_NONE;
        }
    }

    return ZP_COLLISION_RESULT_CONTAINS;
}
