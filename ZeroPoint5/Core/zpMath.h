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

//
// Functions
//

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
};

#endif // !ZP_MATH_H
