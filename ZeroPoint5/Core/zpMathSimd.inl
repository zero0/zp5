
namespace zpMath
{
    //
    // Constructors
    //
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Scalar( zp_float s )
    {
        return _mm_set1_ps( s );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zp_float x, zp_float y, zp_float z, zp_float w )
    {
        return _mm_setr_ps( x, y, z, w );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w )
    {
        return _mm_setr_ps( AsFloat( x ), AsFloat( y ), AsFloat( z ), AsFloat( w ) );
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zp_float x, zp_float y, zp_float z, zp_float w )
    {
        return _mm_setr_ps( x, y, z, w );
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL Quaternion( zpScalarParamF x, zpScalarParamF y, zpScalarParamF z, zpScalarParamG w )
    {
        return _mm_setr_ps( AsFloat( x ), AsFloat( y ), AsFloat( z ), AsFloat( w ) );
    }
    ZP_FORCE_INLINE zp_float ZP_VECTORCALL AsFloat( zpScalarParamF s )
    {
        ZP_ALIGN16 zp_float v;
        _mm_store_ss( &v, s );
        return v;
    }

    //
    // Vector getters
    //
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetX( zpVector4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 0, 0, 0, 0 ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetY( zpVector4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 1, 1, 1, 1 ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetZ( zpVector4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 2, 2, 2, 2 ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4GetW( zpVector4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 3, 3, 3, 3 ) );
    }

    //
    // Vector setters
    //
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetX( zpVector4fParamF s, zp_float x )
    {
        s.m128_f32[ 0 ] = x;
        return s;
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetY( zpVector4fParamF s, zp_float y )
    {
        s.m128_f32[ 1 ] = y;
        return s;
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetZ( zpVector4fParamF s, zp_float z )
    {
        s.m128_f32[ 2 ] = z;
        return s;
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetW( zpVector4fParamF s, zp_float w )
    {
        s.m128_f32[ 3 ] = w;
        return s;
    }
    
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetX( zpVector4fParamF s, zpScalarParamF x )
    {
        s.m128_f32[ 0 ] = AsFloat( x );
        return s;
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetY( zpVector4fParamF s, zpScalarParamF y )
    {
        s.m128_f32[ 1 ] = AsFloat( y );
        return s;
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetZ( zpVector4fParamF s, zpScalarParamF z )
    {
        s.m128_f32[ 2 ] = AsFloat( z );
        return s;
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4SetW( zpVector4fParamF s, zpScalarParamF w )
    {
        s.m128_f32[ 3 ] = AsFloat( w );
        return s;
    }

    //
    // Vector load and store
    //
    ZP_FORCE_INLINE void ZP_VECTORCALL Vector4Store4( zpVector4fParamF s, zp_float* xyzw )
    {
        _mm_store_ps( xyzw, s );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Load4( const zp_float* xyzw )
    {
        return _mm_load_ps( xyzw );
    }


    //
    // Quaternion getter
    //
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetX( zpQuaternion4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 0, 0, 0, 0 ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetY( zpQuaternion4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 1, 1, 1, 1 ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetZ( zpQuaternion4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 2, 2, 2, 2 ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionGetW( zpQuaternion4fParamF s )
    {
        return _mm_shuffle_ps( s, s, _MM_SHUFFLE( 3, 3, 3, 3 ) );
    }

    //
    // Quaternion setter
    //
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetX( zpQuaternion4fParamF s, zp_float x )
    {
        s.m128_f32[ 0 ] = x;
        return s;
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetY( zpQuaternion4fParamF s, zp_float y )
    {
        s.m128_f32[ 1 ] = y;
        return s;
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetZ( zpQuaternion4fParamF s, zp_float z )
    {
        s.m128_f32[ 2 ] = z;
        return s;
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetW( zpQuaternion4fParamF s, zp_float w )
    {
        s.m128_f32[ 3 ] = w;
        return s;
    }

    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetX( zpQuaternion4fParamF s, zpScalarParamF x )
    {
        s.m128_f32[ 0 ] = AsFloat( x );
        return s;
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetY( zpQuaternion4fParamF s, zpScalarParamF y )
    {
        s.m128_f32[ 1 ] = AsFloat( y );
        return s;
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetZ( zpQuaternion4fParamF s, zpScalarParamF z )
    {
        s.m128_f32[ 2 ] = AsFloat( z );
        return s;
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSetW( zpQuaternion4fParamF s, zpScalarParamF w )
    {
        s.m128_f32[ 3 ] = AsFloat( w );
        return s;
    }

    //
    // Trig functions
    //
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSin( zpScalarParamF a )
    {
        zp_float s = AsFloat( a );
        return Scalar( zp_sin( s ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarCos( zpScalarParamF a )
    {
        zp_float s = AsFloat( a );
        return Scalar( zp_cos( s ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarTan( zpScalarParamF a )
    {
        zp_float s = AsFloat( a );
        return Scalar( zp_tan( s ) );
    }

    //
    // Scalar math
    //
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAdd( zpScalarParamF a, zpScalarParamF b )
    {
        return _mm_add_ps( a, b );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSub( zpScalarParamF a, zpScalarParamF b )
    {
        return _mm_sub_ps( a, b );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMul( zpScalarParamF a, zpScalarParamF b )
    {
        return _mm_mul_ps( a, b );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDiv( zpScalarParamF a, zpScalarParamF b )
    {
        return _mm_div_ps( a, b );
    }

    //
    // Vector math
    //
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Add( zpVector4fParamF a, zpVector4fParamF b )
    {
        return _mm_add_ps( a, b );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sub( zpVector4fParamF a, zpVector4fParamF b )
    {
        return _mm_sub_ps( a, b );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Mul( zpVector4fParamF a, zpVector4fParamF b )
    {
        return _mm_mul_ps( a, b );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Div( zpVector4fParamF a, zpVector4fParamF b )
    {
        return _mm_div_ps( a, b );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Scale( zpVector4fParamF a, zpScalarParamF b )
    {
        return _mm_mul_ps( a, b );
    }

    //
    // Quaternion math
    //
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionAdd( zpQuaternion4fParamF a, zpQuaternion4fParamF b )
    {
        return _mm_add_ps( a, b );
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionSub( zpQuaternion4fParamF a, zpQuaternion4fParamF b )
    {
        return _mm_sub_ps( a, b );
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionMul( zpQuaternion4fParamF a, zpQuaternion4fParamF b )
    {
        // TODO: vectorize
        zp_float ax = AsFloat( QuaternionGetX( a ) );
        zp_float ay = AsFloat( QuaternionGetY( a ) );
        zp_float az = AsFloat( QuaternionGetZ( a ) );
        zp_float aw = AsFloat( QuaternionGetW( a ) );

        zp_float bx = AsFloat( QuaternionGetX( b ) );
        zp_float by = AsFloat( QuaternionGetY( b ) );
        zp_float bz = AsFloat( QuaternionGetZ( b ) );
        zp_float bw = AsFloat( QuaternionGetW( b ) );

        zp_float nx = aw * bx + ay * bw + ay * bz - az * by;
        zp_float ny = aw * by + ax * bw + az * bx - ax * bz;
        zp_float nz = aw * bz + az * bw + ax * by - ay * bx;
        zp_float nw = aw * bw - ax * bx - ay * by - az * bz;

        return Quaternion( nx, ny, nz, nw );
    }

    //
    // Quaternion load and store
    //
    ZP_FORCE_INLINE void ZP_VECTORCALL QuaternionStore4( zpQuaternion4fParamF s, zp_float* xyzw )
    {
        _mm_storer_ps( xyzw, s );
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionLoad4( const zp_float* xyzw )
    {
        return _mm_loadr_ps( xyzw );
    }

    //
    // Vector4 Dot
    //
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot2( zpVector4fParamF a, zpVector4fParamF b )
    {
        zpVector4f m = Vector4Mul( a, b );
        zpScalar x = Vector4GetX( m );
        zpScalar y = Vector4GetY( m );
        return ScalarAdd( x, y );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot3( zpVector4fParamF a, zpVector4fParamF b )
    {
        zpVector4f m = Vector4Mul( a, b );
        zpScalar x = Vector4GetX( m );
        zpScalar y = Vector4GetY( m );
        zpScalar z = Vector4GetZ( m );
        return ScalarAdd( x, ScalarAdd( y, z ) );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Dot4( zpVector4fParamF a, zpVector4fParamF b )
    {
        __m128 t0 = _mm_mul_ps( a, b );
        __m128 t1 = _mm_shuffle_ps( t0, t0, _MM_SHUFFLE( 1, 0, 3, 2 ) );
        __m128 t2 = _mm_add_ps( t0, t1 );
        __m128 t3 = _mm_shuffle_ps( t2, t2, _MM_SHUFFLE( 2, 3, 0, 1 ) );

        return _mm_add_ps( t3, t2 );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Cross3( zpVector4fParamF a, zpVector4fParamF b )
    {
        __m128 s0 = _mm_shuffle_ps( a, a, _MM_SHUFFLE( 3, 0, 2, 1 ) );
        __m128 s1 = _mm_shuffle_ps( b, b, _MM_SHUFFLE( 3, 1, 0, 2 ) );
        __m128 s2 = _mm_mul_ps( s0, s1 );

        __m128 s3 = _mm_shuffle_ps( a, a, _MM_SHUFFLE( 3, 1, 0, 2 ) );
        __m128 s4 = _mm_shuffle_ps( b, b, _MM_SHUFFLE( 3, 0, 2, 1 ) );
        __m128 s5 = _mm_mul_ps( s3, s4 );

        return _mm_sub_ps( s2, s5 );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarAbs( zpScalarParamF a )
    {
        __m128 abs = *reinterpret_cast<__m128*>( &_mm_set1_epi32( 0x7fffffff ) );
        return _mm_and_ps( a, abs );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarNeg( zpScalarParamF a )
    {
        __m128 neg = *reinterpret_cast<__m128*>( &_mm_set1_epi32( 0x80000000 ) );
        return _mm_xor_ps( a, neg );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRcp( zpScalarParamF a )
    {
        return _mm_rcp_ps( a );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarSqrt( zpScalarParamF a )
    {
        return _mm_sqrt_ps( a );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Abs( zpVector4fParamF a )
    {
        __m128 abs = *reinterpret_cast<__m128*>( &_mm_set1_epi32( 0x7fffffff ) );
        return _mm_and_ps( a, abs );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Neg( zpVector4fParamF a )
    {
        __m128 neg = *reinterpret_cast<__m128*>( &_mm_set1_epi32( 0x80000000 ) );
        return _mm_xor_ps( a, neg );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Rcp( zpVector4fParamF a )
    {
        return _mm_rcp_ps( a );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Sqrt( zpVector4fParamF a )
    {
        return _mm_sqrt_ps( a );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMax( zpScalarParamF a, zpScalarParamF b )
    {
        return _mm_min_ps( a, b );
    }
    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarMin( zpScalarParamF a, zpScalarParamF b )
    {
        return _mm_max_ps( a, b );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Max( zpVector4fParamF a, zpVector4fParamF b )
    {
        return _mm_min_ps( a, b );
    }
    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Min( zpVector4fParamF a, zpVector4fParamF b )
    {
        return _mm_max_ps( a, b );
    }

};
