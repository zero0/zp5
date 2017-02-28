#pragma once
#ifndef ZP_MATH_H
#define ZP_MATH_H

//
// Defines
//

#define ZP_EPSILON      ( 1e-8f )
#define ZP_PI           ( 3.14159265359f )
#define ZP_1OVERPI      ( 1.0f / ZP_PI )
#define ZP_2PI          ( 2.0f * ZP_PI )
#define ZP_PIOVER2      ( 0.5f * ZP_PI )
#define ZP_PIOVER4      ( 0.25f* ZP_PI ) 
#define ZP_FLT_MAX      ( 3.402823466e+38F )
#define ZP_FLT_MIN      ( 1.175494351e-38F )
#define ZP_PIOVER180    ( ZP_PI / 180.f )
#define ZP_180OVERPI    ( 180.f / ZP_PI )

#define ZP_RAD_TO_DEG( r )    ( (r) * ZP_180OVERPI )
#define ZP_DEG_TO_RAD( d )    ( (d) * ZP_PIOVER180 )

//
// Types
//

typedef union
{
    zp_float v[ 4 ];
    struct
    {
        zp_float x, y, z, w;
    };
} zp_vec4;

#ifdef ZP_USE_SIMD
#include <immintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

typedef __m128 zpScalar;
typedef __m128 zpVector4f;
typedef __m128 zpQuaternion4f;
typedef union ZP_ALIGN16
{
    __m128 m_r[ 4 ];
    struct
    {
        __m128 m_m1;
        __m128 m_m2;
        __m128 m_m3;
        __m128 m_m4;
    };
} zpMatrix4f;

#ifdef ZP_WIN_32
typedef zpScalar                zpScalarParamF; // use for first 3 params
typedef const zpScalar&         zpScalarParamG; // 4th param
typedef const zpScalar&         zpScalarParamH; // 5th and 6th param
typedef const zpScalar&         zpScalarParamC; // all the rest

typedef zpVector4f              zpVector4fParamF;
typedef const zpVector4f&       zpVector4fParamG;
typedef const zpVector4f&       zpVector4fParamH;
typedef const zpVector4f&       zpVector4fParamC;

typedef zpQuaternion4f          zpQuaternion4fParamF;
typedef const zpQuaternion4f&   zpQuaternion4fParamG;
typedef const zpQuaternion4f&   zpQuaternion4fParamH;
typedef const zpQuaternion4f&   zpQuaternion4fParamC;

typedef const zpMatrix4f&       zpMatrix4fParamF;
typedef const zpMatrix4f&       zpMatrix4fParamC;
#elif defined( ZP_WIN_64 )
typedef zpScalar                zpScalarParamF; // use for first 3 params
typedef zpScalar                zpScalarParamG; // 4th param
typedef zpScalar                zpScalarParamH; // 5th and 6th param
typedef const zpScalar&         zpScalarParamC; // all the rest

typedef zpVector4f              zpVector4fParamF;
typedef zpVector4f              zpVector4fParamG;
typedef zpVector4f              zpVector4fParamH;
typedef const zpVector4f&       zpVector4fParamC;

typedef zpQuaternion4f          zpQuaternion4fParamF;
typedef zpQuaternion4f          zpQuaternion4fParamG;
typedef zpQuaternion4f          zpQuaternion4fParamH;
typedef const zpQuaternion4f&   zpQuaternion4fParamC;

typedef zpMatrix4f              zpMatrix4fParamF;
typedef const zpMatrix4f&       zpMatrix4fParamC;
#endif // !ZP_WIN_32 && !ZP_WIN_64

#else // !ZP_USE_SIMD
typedef zp_vec4 zpScalar;
typedef zp_vec4 zpVector4f;
typedef zp_vec4 zpQuaternion4f;
typedef union
{
    zp_vec4 m_r[ 4 ];
    struct
    {
        zp_vec4 m_m1;
        zp_vec4 m_m2;
        zp_vec4 m_m3;
        zp_vec4 m_m4;
    };
} zpMatrix4f;

typedef const zpScalar&         zpScalarParamF;
typedef const zpScalar&         zpScalarParamG;
typedef const zpScalar&         zpScalarParamH;
typedef const zpScalar&         zpScalarParamC;

typedef const zpVector4f&       zpVector4fParamF;
typedef const zpVector4f&       zpVector4fParamG;
typedef const zpVector4f&       zpVector4fParamH;
typedef const zpVector4f&       zpVector4fParamC;

typedef const zpQuaternion4f&   zpQuaternion4fParamF;
typedef const zpQuaternion4f&   zpQuaternion4fParamG;
typedef const zpQuaternion4f&   zpQuaternion4fParamH;
typedef const zpQuaternion4f&   zpQuaternion4fParamC;

typedef const zpMatrix4f&       zpMatrix4fParamF;
typedef const zpMatrix4f&       zpMatrix4fParamC;

#endif // ZP_USE_SIMD

struct zpScalarData
{
    union
    {
        struct
        {
            zp_float v, r0, r1, r2;
        };
        zp_float m[ 4 ];
    };
};

struct zpVector4fData
{
    union
    {
        struct
        {
            zp_float x, y, z, w;
        };
        zp_float m[ 4 ];
    };
};

struct zpQuaternion4fData
{
    union
    {
        struct
        {
            zp_float x, y, z, w;
        };
        zp_float m[ 4 ];
    };
};

struct zpMatrix4fData
{
    zp_float m[ 16 ];
};

struct zpVector2f
{
    zp_float x, y;
};

struct zpVector2i
{
    zp_int x, y;
};

struct zpRectf
{
    zp_float x, y, width, height;
};

struct zpRecti
{
    zp_int x, y, width, height;
};

struct zpColorf
{
    union
    {
        struct
        {
            zp_float r, g, b, a;
        };
        zp_float rgba[4];
    };
};

struct zpColor32i
{
    union
    {
        struct
        {
            zp_byte r, g, b, a;
        };
        zp_uint rgba;
    };
};

struct zpVector4fCmp
{
    union
    {
        struct
        {
            zp_int cmpX;
            zp_int cmpY;
            zp_int cmpZ;
            zp_int cmpW;
        };
        zp_int cmp[ 4 ];
    };
};

//
// Collision
//

enum zpCollisionResult
{
    ZP_COLLISION_RESULT_NONE = 0,
    ZP_COLLISION_RESULT_INTERSECT,
    ZP_COLLISION_RESULT_CONTAINS
};

struct zpRay
{
    zpVector4fData position;
    zpVector4fData direction;
};

struct zpBoundingAABB
{
    zpVector4fData center;
    zpVector4fData extents;
};

struct zpBoundingSphere
{
    zpVector4fData center;
    zp_float radius;
};

struct zpPlane
{
    zpVector4fData normal;
    zp_float d;
};

struct zpFrustum
{
    union
    {
        struct
        {
            zpPlane left,
                    right,
                    bottom,
                    top,
                    near,
                    far;
        };
        zpPlane planes[ 6 ];
    };

    union
    {
        struct
        {
            zpVector4fData ntl, ntr, nbl, nbr;
            zpVector4fData ftl, ftr, fbl, fbr;
        };
        zpVector4fData points[ 8 ];
    };
};

//
// Functions
//

zp_float zp_cos( zp_float r );
zp_float zp_sin( zp_float r );
zp_float zp_tan( zp_float r );

namespace zpMath
{
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Scalar( zp_float s );
    ZP_FORCE_INLINE zp_float ZP_VECTORCALL AsFloat( zpScalarParamF s );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zp_float x, zp_float y, zp_float z, zp_float w );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zp_float x, zp_float y, zp_float z, zp_float w );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetX( zpVector4fParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetY( zpVector4fParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetZ( zpVector4fParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetW( zpVector4fParamF s );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetX( zpVector4fParamF s, zp_float x );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetY( zpVector4fParamF s, zp_float y );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetZ( zpVector4fParamF s, zp_float z );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetW( zpVector4fParamF s, zp_float w );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetX( zpVector4fParamF s, zpScalarParamF x );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetY( zpVector4fParamF s, zpScalarParamF y );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetZ( zpVector4fParamF s, zpScalarParamF z );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetW( zpVector4fParamF s, zpScalarParamF w );

    ZP_FORCE_INLINE void ZP_VECTORCALL Vector4Store4( zpVector4fParamF s, zp_float* xyzw );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Load4( const zp_float* xyzw );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetX( zpQuaternion4fParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetY( zpQuaternion4fParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetZ( zpQuaternion4fParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetW( zpQuaternion4fParamF s );

    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetX( zpQuaternion4fParamF s, zp_float x );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetY( zpQuaternion4fParamF s, zp_float y );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetZ( zpQuaternion4fParamF s, zp_float z );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetW( zpQuaternion4fParamF s, zp_float w );

    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetX( zpQuaternion4fParamF s, zpScalarParamF x );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetY( zpQuaternion4fParamF s, zpScalarParamF y );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetZ( zpQuaternion4fParamF s, zpScalarParamF z );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetW( zpQuaternion4fParamF s, zpScalarParamF w );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSin( zpScalarParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarCos( zpScalarParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarTan( zpScalarParamF a );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAdd( zpScalarParamF a, zpScalarParamF b );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSub( zpScalarParamF a, zpScalarParamF b );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMul( zpScalarParamF a, zpScalarParamF b );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDiv( zpScalarParamF a, zpScalarParamF b );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDegToRad( zpScalarParamF s );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRadToDeg( zpScalarParamF s );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Add( zpVector4fParamF a, zpVector4fParamF b );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sub( zpVector4fParamF a, zpVector4fParamF b );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Mul( zpVector4fParamF a, zpVector4fParamF b );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Div( zpVector4fParamF a, zpVector4fParamF b );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Scale( zpVector4fParamF a, zpScalarParamF b );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared2( zpVector4fParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared3( zpVector4fParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared4( zpVector4fParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionLengthSquared4( zpQuaternion4fParamF a );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length2( zpVector4fParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length3( zpVector4fParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length4( zpVector4fParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionLength4( zpQuaternion4fParamF a );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize2( zpVector4fParamF a );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize3( zpVector4fParamF a );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize4( zpVector4fParamF a );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionNormalize4( zpQuaternion4fParamF a );

    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionAdd( zpQuaternion4fParamF a, zpQuaternion4fParamF b );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSub( zpQuaternion4fParamF a, zpQuaternion4fParamF b );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionMul( zpQuaternion4fParamF a, zpQuaternion4fParamF b );

    ZP_FORCE_INLINE void ZP_VECTORCALL QuaternionStore4( zpQuaternion4f s, zp_float* xyzw );
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionLoad4( const zp_float* xyzw );

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL QuaternionToMatrix( zpQuaternion4fParamF a );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot2( zpVector4fParamF a, zpVector4fParamF b );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot3( zpVector4fParamF a, zpVector4fParamF b );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot4( zpVector4fParamF a, zpVector4fParamF b );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Cross3( zpVector4fParamF a, zpVector4fParamF b );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAbs( zpScalarParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarNeg( zpScalarParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRcp( zpScalarParamF a );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSqrt( zpScalarParamF a );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Abs( zpVector4fParamF a );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Neg( zpVector4fParamF a );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Rcp( zpVector4fParamF a );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sqrt( zpVector4fParamF a );

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMax( zpScalarParamF a, zpScalarParamF b );
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMin( zpScalarParamF a, zpScalarParamF b );

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Max( zpVector4fParamF a, zpVector4fParamF b );
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Min( zpVector4fParamF a, zpVector4fParamF b );

    ZP_FORCE_INLINE zp_int ZP_VECTORCALL ScalarCmp( zpScalarParamF a, zpScalarParamF b );
    ZP_FORCE_INLINE zpVector4fCmp ZP_VECTORCALL Vector4Cmp( zpVector4fParamF a, zpVector4fParamF b );
    
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixT( zpVector4fParamF p );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixTR( zpVector4fParamF p, zpQuaternion4fParamF r );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixTRS( zpVector4fParamF p, zpQuaternion4fParamF r, zpVector4fParamF s );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixIdentity();

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixMul( zpMatrix4fParamF a, zpMatrix4fParamC b );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixTranspose( zpMatrix4fParamF a );

    ZP_FORCE_INLINE void ZP_VECTORCALL MatrixStore4( zpMatrix4fParamF s, zp_float* m );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixLoad4( const zp_float* m );

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtLH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtRH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up );
    
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveLH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveRH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveLH( zp_float fovy, zp_float aspectRatio, zp_float zNear, zp_float zFar );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveRH( zp_float fovy, zp_float aspectRatio, zp_float zNear, zp_float zFar );

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoLH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoRH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoLH( zp_float l, zp_float r, zp_float t, zp_float b, zp_float zNear, zp_float zFar );
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoRH( zp_float l, zp_float r, zp_float t, zp_float b, zp_float zNear, zp_float zFar );
};

//
// Non-SIMD Functions
//

namespace zpMath
{
    void PlaneSet( zpPlane& plane, zp_float a, zp_float b, zp_float c, zp_float d );
    void PlaneSet( zpPlane& plane, zpVector4fParamF p0, zpVector4fParamF p1, zpVector4fParamF p2 );
    void PlaneSet( zpPlane& plane, const zpVector4fData& p0, const zpVector4fData& p1, const zpVector4fData& p2 );

    zp_int PlaneGetSide( const zpPlane& plane, const zpVector4fData& p );
    zp_float PlaneDistanceToPoint( const zpPlane& plane, const zpVector4fData& p );

    void FrustrumSetLookTo( zpFrustum& frustrum, const zpVector4fData& eye, const zpVector4fData& direction, const zpVector4fData& up, zp_float ratio, zp_float fovy, zp_float zNear, zp_float zFar );
}

//
// Collision
//

namespace zpCollision
{
    // AABB
    zpCollisionResult Test( const zpBoundingAABB& bounds, const zpVector4fData& pos );
    zpCollisionResult Test( const zpBoundingAABB& bounds, const zpRay& ray );
    zpCollisionResult Test( const zpBoundingAABB& bounds, const zpBoundingAABB& box );
    zpCollisionResult Test( const zpBoundingAABB& bounds, const zpBoundingSphere& sphere );

    // Sphere
    zpCollisionResult Test( const zpBoundingSphere& bounds, const zpVector4fData& pos );
    zpCollisionResult Test( const zpBoundingSphere& bounds, const zpRay& ray );
    zpCollisionResult Test( const zpBoundingSphere& bounds, const zpBoundingAABB& box );
    zpCollisionResult Test( const zpBoundingSphere& bounds, const zpBoundingSphere& sphere );

    // Frustum
    zpCollisionResult Test( const zpFrustum& a, const zpVector4fData& pos );
    zpCollisionResult Test( const zpFrustum& a, const zpRay& b );
    zpCollisionResult Test( const zpFrustum& a, const zpBoundingAABB& b );
    zpCollisionResult Test( const zpFrustum& a, const zpBoundingSphere& b );
};

//
// Colors
//

namespace zpColor
{
    const zpColorf Clear =     { 0, 0, 0, 0 };
    const zpColorf Invisible = { 1, 1, 1, 0 };

    const zpColorf White = { 1, 1, 1, 1 };
    const zpColorf Black = { 0, 0, 0, 1 };

    const zpColorf Red =   { 1, 0, 0, 1 };
    const zpColorf Green = { 0, 1, 0, 1 };
    const zpColorf Blue =  { 0, 0, 1, 1 };

    const zpColorf Magenta = { 1, 0, 1, 1 };
    const zpColorf Yellow =  { 1, 1, 0, 1 };
    const zpColorf Cyan =    { 0, 1, 1, 1 };
};

namespace zpColor32
{
    const zpColor32i Clear =     { 0x00, 0x00, 0x00, 0x00 };
    const zpColor32i Invisible = { 0xFF, 0xFF, 0xFF, 0x00 };

    const zpColor32i White = { 0xFF, 0xFF, 0xFF, 0xFF };
    const zpColor32i Black = { 0x00, 0x00, 0x00, 0xFF };

    const zpColor32i Red =   { 0xFF, 0x00, 0x00, 0xFF };
    const zpColor32i Green = { 0x00, 0xFF, 0x00, 0xFF };
    const zpColor32i Blue =  { 0x00, 0x00, 0xFF, 0xFF };

    const zpColor32i Magenta = { 0xFF, 0x00, 0xFF, 0xFF };
    const zpColor32i Yellow =  { 0xFF, 0xFF, 0x00, 0xFF };
    const zpColor32i Cyan =    { 0x00, 0xFF, 0xFF, 0xFF };
};

#ifdef ZP_USE_SIMD
#include "zpMathSimd.inl"
#else // !ZP_USE_SIMD
#include "zpMathFpu.inl"
#endif // ZP_USE_SIMD

#endif // !ZP_MATH_H
