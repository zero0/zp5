
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

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarDegToRad( zpScalarParamF a )
    {
        return ScalarMul( a, Scalar( ZP_PIOVER180 ) );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL ScalarRadToDeg( zpScalarParamF a )
    {
        return ScalarMul( a, Scalar( ZP_180OVERPI ) );
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

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared2( zpVector4fParamF a )
    {
        return Vector4Dot2( a, a );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared3( zpVector4fParamF a )
    {
        return Vector4Dot3( a, a );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4LengthSquared4( zpVector4fParamF a )
    {
        return Vector4Dot4( a, a );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length2( zpVector4fParamF a )
    {
        return ScalarSqrt( Vector4Dot2( a, a ) );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length3( zpVector4fParamF a )
    {
        return ScalarSqrt( Vector4Dot3( a, a ) );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL Vector4Length4( zpVector4fParamF a )
    {
        return ScalarSqrt( Vector4Dot4( a, a ) );
    }

    ZP_FORCE_INLINE zpScalar ZP_VECTORCALL QuaternionLength4( zpQuaternion4fParamF a )
    {
        return ScalarSqrt( Scalar( 0 ) );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize2( zpVector4fParamF a )
    {
        zpScalar l = Vector4Length2( a );
        return ScalarDiv( a, l );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize3( zpVector4fParamF a )
    {
        zpScalar l = Vector4Length3( a );
        return ScalarDiv( a, l );
    }

    ZP_FORCE_INLINE zpVector4f ZP_VECTORCALL Vector4Normalize4( zpVector4fParamF a )
    {
        zpScalar l = Vector4Length4( a );
        return ScalarDiv( a, l );
    }

    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionNormalize4( zpQuaternion4fParamF a )
    {
        zpScalar l = QuaternionLength4( a );
        return ScalarDiv( a, l );
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
        _mm_store_ps( xyzw, s );
    }
    ZP_FORCE_INLINE zpQuaternion4f ZP_VECTORCALL QuaternionLoad4( const zp_float* xyzw )
    {
        return _mm_load_ps( xyzw );
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL QuaternionToMatrix( zpQuaternion4fParamF a )
    {
        //zpScalar x = QuaternionGetX( a );
        //zpScalar y = QuaternionGetY( a );
        //zpScalar z = QuaternionGetZ( a );
        //zpScalar w = QuaternionGetW( a );
        //
        //zpScalar xx = ScalarMul( x, x );
        //zpScalar xy = ScalarMul( x, y );
        //zpScalar xz = ScalarMul( x, z );
        //zpScalar xw = ScalarMul( x, w );
        //zpScalar yy = ScalarMul( y, y );
        //zpScalar yz = ScalarMul( y, z );
        //zpScalar yw = ScalarMul( y, w );
        //zpScalar zz = ScalarMul( z, z );
        //zpScalar zw = ScalarMul( z, w );
        //zpScalar zr = Scalar( 0 );

        zp_float x = AsFloat( QuaternionGetX( a ) );
        zp_float y = AsFloat( QuaternionGetY( a ) );
        zp_float z = AsFloat( QuaternionGetZ( a ) );
        zp_float w = AsFloat( QuaternionGetW( a ) );

        zp_float xx = ( x * x );
        zp_float xy = ( x * y );
        zp_float xz = ( x * z );
        zp_float xw = ( x * w );
        zp_float yy = ( y * y );
        zp_float yz = ( y * z );
        zp_float yw = ( y * w );
        zp_float zz = ( z * z );
        zp_float zw = ( z * w );
        zp_float zr = ( 0.f );

        zpVector4f m0 = Vector4Add( Vector4( yy, xy, xz, zr ), Vector4( zz, zw, -( yw ), zr ) );
        zpVector4f m1 = Vector4Add( Vector4( xy, xx, yz, zr ), Vector4( -( zw ), zz, xw, zr ) );
        zpVector4f m2 = Vector4Add( Vector4( xz, yz, xx, zr ), Vector4( yw, -( xw ), yy, zr ) );

        zpMatrix4f m;
        m.m_m1 = Vector4Add( Vector4( 1, 0, 0, 0 ), Vector4Mul( m0, Vector4( -2, 2, 2, 0 ) ) );
        m.m_m2 = Vector4Add( Vector4( 0, 1, 0, 0 ), Vector4Mul( m1, Vector4( 2, -2, 2, 0 ) ) );
        m.m_m3 = Vector4Add( Vector4( 0, 0, 1, 0 ), Vector4Mul( m2, Vector4( 2, 2, -2, 0 ) ) );
        m.m_m4 = Vector4( 0, 0, 0, 1 );

        return m;
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

    ZP_FORCE_INLINE zp_int ZP_VECTORCALL ScalarCmp( zpScalarParamF a, zpScalarParamF b )
    {
        __m128 lt = _mm_cmplt_ss( a, b );
        __m128 gt = _mm_cmpgt_ss( a, b );

        return lt.m128_i32[ 0 ] ? -1 : gt.m128_i32[ 0 ] ? 1 : 0;
    }

    ZP_FORCE_INLINE zpVector4fCmp ZP_VECTORCALL Vector4Cmp( zpVector4fParamF a, zpVector4fParamF b )
    {
        __m128 lt = _mm_cmplt_ps( a, b );
        __m128 gt = _mm_cmpgt_ps( a, b );

        zpVector4fCmp cmp;
        cmp.cmp[ 0 ] = lt.m128_i32[ 0 ] ? -1 : gt.m128_i32[ 0 ] ? 1 : 0;
        cmp.cmp[ 1 ] = lt.m128_i32[ 1 ] ? -1 : gt.m128_i32[ 1 ] ? 1 : 0;
        cmp.cmp[ 2 ] = lt.m128_i32[ 2 ] ? -1 : gt.m128_i32[ 2 ] ? 1 : 0;
        cmp.cmp[ 3 ] = lt.m128_i32[ 3 ] ? -1 : gt.m128_i32[ 3 ] ? 1 : 0;
        return cmp;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixT( zpVector4fParamF p )
    {
        zpMatrix4f m;
        m.m_m1 = _mm_setr_ps( 1, 0, 0, 0 );
        m.m_m2 = _mm_setr_ps( 0, 1, 0, 0 );
        m.m_m3 = _mm_setr_ps( 0, 0, 1, 0 );
        m.m_m4 = p;

        return m;
    }
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixTR( zpVector4fParamF p, zpQuaternion4fParamF r )
    {
        zpMatrix4f pm;
        pm.m_m1 = _mm_setr_ps( 1, 0, 0, 0 );
        pm.m_m2 = _mm_setr_ps( 0, 1, 0, 0 );
        pm.m_m3 = _mm_setr_ps( 0, 0, 1, 0 );
        pm.m_m4 = p;

        zpMatrix4f rm = QuaternionToMatrix( r );

        zpMatrix4f m;
        m = MatrixMul( rm, pm );

        return m;
    }
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixTRS( zpVector4fParamF p, zpQuaternion4fParamF r, zpVector4fParamF s )
    {
        zpMatrix4f pm;
        pm.m_m1 = _mm_setr_ps( 1, 0, 0, 0 );
        pm.m_m2 = _mm_setr_ps( 0, 1, 0, 0 );
        pm.m_m3 = _mm_setr_ps( 0, 0, 1, 0 );
        pm.m_m4 = p;

        zpMatrix4f rm = QuaternionToMatrix( r );

        zpMatrix4f sm = MatrixIdentity();
        sm.m_m1 = Vector4Mul( sm.m_m1, s );
        sm.m_m2 = Vector4Mul( sm.m_m2, s );
        sm.m_m3 = Vector4Mul( sm.m_m3, s );

        zpMatrix4f m;
        m = MatrixMul( sm, MatrixMul( rm, pm ) );

        return m;
    }
    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixIdentity()
    {
        zpMatrix4f m;
        m.m_m1 = _mm_setr_ps( 1, 0, 0, 0 );
        m.m_m2 = _mm_setr_ps( 0, 1, 0, 0 );
        m.m_m3 = _mm_setr_ps( 0, 0, 1, 0 );
        m.m_m4 = _mm_setr_ps( 0, 0, 0, 1 );

        return m;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixMul( zpMatrix4fParamF a, zpMatrix4fParamC b )
    {
        zpMatrix4f s;

        zpVector4f col1 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m1 ) );
        zpVector4f col2 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m2 ) );
        zpVector4f col3 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m3 ) );
        zpVector4f col4 = Vector4Scale( b.m_m1, Vector4GetX( a.m_m4 ) );

        col1 = Vector4Add( col1, Vector4Scale( b.m_m2, Vector4GetY( a.m_m1 ) ) );
        col2 = Vector4Add( col2, Vector4Scale( b.m_m2, Vector4GetY( a.m_m2 ) ) );
        col3 = Vector4Add( col3, Vector4Scale( b.m_m2, Vector4GetY( a.m_m3 ) ) );
        col4 = Vector4Add( col4, Vector4Scale( b.m_m2, Vector4GetY( a.m_m4 ) ) );

        col1 = Vector4Add( col1, Vector4Scale( b.m_m3, Vector4GetZ( a.m_m1 ) ) );
        col2 = Vector4Add( col2, Vector4Scale( b.m_m3, Vector4GetZ( a.m_m2 ) ) );
        col3 = Vector4Add( col3, Vector4Scale( b.m_m3, Vector4GetZ( a.m_m3 ) ) );
        col4 = Vector4Add( col4, Vector4Scale( b.m_m3, Vector4GetZ( a.m_m4 ) ) );

        s.m_m1 = Vector4Add( col1, Vector4Scale( b.m_m4, Vector4GetW( a.m_m1 ) ) );
        s.m_m2 = Vector4Add( col2, Vector4Scale( b.m_m4, Vector4GetW( a.m_m2 ) ) );
        s.m_m3 = Vector4Add( col3, Vector4Scale( b.m_m4, Vector4GetW( a.m_m3 ) ) );
        s.m_m4 = Vector4Add( col4, Vector4Scale( b.m_m4, Vector4GetW( a.m_m4 ) ) );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixTranspose( zpMatrix4fParamF a )
    {
        zpMatrix4f s = a;
        _MM_TRANSPOSE4_PS( s.m_m1, s.m_m2, s.m_m3, s.m_m4 );

        return s;
    }

    ZP_FORCE_INLINE void ZP_VECTORCALL MatrixStore4( zpMatrix4fParamF s, zp_float* m )
    {
        _mm_store_ps( m + 0, s.m_m1 );
        _mm_store_ps( m + 4, s.m_m2 );
        _mm_store_ps( m + 8, s.m_m3 );
        _mm_store_ps( m + 12, s.m_m4 );
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL MatrixLoad4( const zp_float* m )
    {
        zpMatrix4f mat;
        mat.m_m1 = _mm_load_ps( m + 0 );
        mat.m_m2 = _mm_load_ps( m + 4 );
        mat.m_m3 = _mm_load_ps( m + 8 );
        mat.m_m4 = _mm_load_ps( m + 12 );

        return mat;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtLH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up )
    {
        zpVector4f x, y, z;
        zpScalar ex, ey, ez;
        zpMatrix4f s;

        z = Vector4Normalize3( direction );
        x = Vector4Cross3( up, z );
        x = Vector4Normalize3( x );
        y = Vector4Cross3( z, x );

        ex = Vector4Dot3( x, eye );
        ey = Vector4Dot3( y, eye );
        ez = Vector4Dot3( z, eye );

        ex = Vector4Neg( ex );
        ey = Vector4Neg( ey );
        ez = Vector4Neg( ez );

        s.m_m1 = x;
        s.m_m2 = y;
        s.m_m3 = z;
        s.m_m4 = Vector4( 0, 0, 0, 0 );

        s = MatrixTranspose( s );

        s.m_m4 = Vector4( ex, ey, ez, Scalar( 1.0f ) );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL LookAtRH( zpVector4fParamF eye, zpVector4fParamF direction, zpVector4fParamF up )
    {
        zpVector4f x, y, z;
        zpScalar ex, ey, ez;
        zpMatrix4f s;

        z = Vector4Neg( direction );
        z = Vector4Normalize3( z );
        x = Vector4Cross3( up, z );
        x = Vector4Normalize3( x );
        y = Vector4Cross3( z, x );

        ex = Vector4Dot3( x, eye );
        ey = Vector4Dot3( y, eye );
        ez = Vector4Dot3( z, eye );

        s.m_m1 = x;
        s.m_m2 = y;
        s.m_m3 = z;
        s.m_m4 = Vector4( 0, 0, 0, 0 );

        s = MatrixTranspose( s );

        s.m_m4 = Vector4( ex, ey, ez, Scalar( 1.0f ) );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveLH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar )
    {
        zpScalar yScale, xScale, z, fn, r22, r32, o;
        z = Scalar( 0.f );
        o = Scalar( 1.f );

        yScale = ScalarDegToRad( fovy );
        yScale = ScalarMul( yScale, Scalar( 0.5f ) );
        yScale = ScalarTan( yScale );
        yScale = ScalarRcp( yScale );

        xScale = ScalarDiv( yScale, aspectRatio );

        fn = ScalarSub( zFar, zNear );

        r22 = ScalarDiv( zFar, fn );

        r32 = ScalarMul( zNear, r22 );
        r32 = ScalarNeg( r32 );

        zpMatrix4f s;
        s.m_m1 = Vector4( xScale, z, z, z );
        s.m_m2 = Vector4( z, yScale, z, z );
        s.m_m3 = Vector4( z, z, r22, o );
        s.m_m4 = Vector4( z, z, r32, z );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveRH( zpScalarParamF fovy, zpScalarParamF aspectRatio, zpScalarParamF zNear, zpScalarParamG zFar )
    {
        zpScalar yScale, xScale, z, nf, r22, r32, no;
        z = Scalar( 0.f );
        no = Scalar( -1.f );

        yScale = ScalarDegToRad( fovy );
        yScale = ScalarMul( yScale, Scalar( 0.5f ) );
        yScale = ScalarTan( yScale );
        yScale = ScalarRcp( yScale );

        xScale = ScalarDiv( yScale, aspectRatio );

        nf = ScalarSub( zNear, zFar );

        r22 = ScalarDiv( zFar, nf );

        r32 = ScalarMul( zNear, r22 );

        zpMatrix4f s;
        s.m_m1 = Vector4( xScale, z, z, z );
        s.m_m2 = Vector4( z, yScale, z, z );
        s.m_m3 = Vector4( z, z, r22, no );
        s.m_m4 = Vector4( z, z, r32, z );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveLH( zp_float fovy, zp_float aspectRatio, zp_float zNear, zp_float zFar )
    {
        zp_float yScale, xScale, z, fn, r22, r32, o;
        z = ( 0.f );
        o = ( 1.f );

        yScale = ZP_DEG_TO_RAD( fovy );
        yScale = ( yScale * ( 0.5f ) );
        yScale = zp_tan( yScale );
        yScale = ( 1.f / yScale );

        xScale = ( yScale / aspectRatio );

        fn = ( zFar - zNear );

        r22 = ( zFar / fn );

        r32 = ( zNear * r22 );
        r32 = ( -r32 );

        zpMatrix4f s;
        s.m_m1 = Vector4( xScale, z, z, z );
        s.m_m2 = Vector4( z, yScale, z, z );
        s.m_m3 = Vector4( z, z, r22, o );
        s.m_m4 = Vector4( z, z, r32, z );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL PerspectiveRH( zp_float fovy, zp_float aspectRatio, zp_float zNear, zp_float zFar )
    {
        zp_float yScale, xScale, z, nf, r22, r32, no;
        z = ( 0.f );
        no = ( -1.f );

        yScale = ZP_DEG_TO_RAD( fovy );
        yScale = ( yScale * ( 0.5f ) );
        yScale = zp_tan( yScale );
        yScale = ( 1.f / yScale );

        xScale = ( yScale / aspectRatio );

        nf = ( zNear - zFar );

        r22 = ( zFar / nf );

        r32 = ( zNear * r22 );

        zpMatrix4f s;
        s.m_m1 = Vector4( xScale, z, z, z );
        s.m_m2 = Vector4( z, yScale, z, z );
        s.m_m3 = Vector4( z, z, r22, no );
        s.m_m4 = Vector4( z, z, r32, z );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoLH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar )
    {
        zpScalar rml, lmr, rpl, tmb, bmt, tpb, fmn, nmf, h, z, o;
        zpScalar m00, m11, m22, m30, m31, m32;
        h = Scalar( 2.f );
        z = Scalar( 0.f );
        o = Scalar( 1.f );

        rml = ScalarSub( r, l );
        lmr = ScalarSub( l, r );
        tmb = ScalarSub( t, b );
        bmt = ScalarSub( b, t );
        fmn = ScalarSub( zFar, zNear );
        nmf = ScalarSub( zNear, zFar );

        rpl = ScalarAdd( r, l );
        tpb = ScalarAdd( t, b );

        // scale
        m00 = ScalarDiv( h, rml );
        m11 = ScalarDiv( h, tmb );
        m22 = ScalarRcp( fmn );

        // translate
        m30 = ScalarDiv( rpl, lmr );
        m31 = ScalarDiv( tpb, bmt );
        m32 = ScalarDiv( zNear, nmf );

        zpMatrix4f s;
        s.m_m1 = Vector4( m00, z, z, z );
        s.m_m2 = Vector4( z, m11, z, z );
        s.m_m3 = Vector4( z, z, m22, z );
        s.m_m4 = Vector4( m30, m31, m32, o );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoRH( zpScalarParamF l, zpScalarParamF r, zpScalarParamF t, zpScalarParamG b, zpScalarParamH zNear, zpScalarParamH zFar )
    {
        zpScalar rml, lmr, rpl, tmb, bmt, tpb, nmf, h, z, o;
        zpScalar m00, m11, m22, m30, m31, m32;
        h = Scalar( 2.f );
        z = Scalar( 0.f );
        o = Scalar( 1.f );

        rml = ScalarSub( r, l );
        lmr = ScalarSub( l, r );
        tmb = ScalarSub( t, b );
        bmt = ScalarSub( b, t );
        nmf = ScalarSub( zNear, zFar );

        rpl = ScalarAdd( r, l );
        tpb = ScalarAdd( t, b );

        // scale
        m00 = ScalarDiv( h, rml );
        m11 = ScalarDiv( h, tmb );
        m22 = ScalarRcp( nmf );

        // translate
        m30 = ScalarDiv( rpl, lmr );
        m31 = ScalarDiv( tpb, bmt );
        m32 = ScalarDiv( zNear, nmf );

        zpMatrix4f s;
        s.m_m1 = Vector4( m00, z, z, z );
        s.m_m2 = Vector4( z, m11, z, z );
        s.m_m3 = Vector4( z, z, m22, z );
        s.m_m4 = Vector4( m30, m31, m32, o );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoLH( zp_float l, zp_float r, zp_float t, zp_float b, zp_float zNear, zp_float zFar )
    {
        zp_float rml, lmr, rpl, tmb, bmt, tpb, fmn, nmf, h, z, o;
        zp_float m00, m11, m22, m30, m31, m32;
        h = ( 2.f );
        z = ( 0.f );
        o = ( 1.f );

        rml = ( r - l );
        lmr = ( l - r );
        tmb = ( t - b );
        bmt = ( b - t );
        fmn = ( zFar - zNear );
        nmf = ( zNear - zFar );

        rpl = ( r + l );
        tpb = ( t + b );

        // scale
        m00 = ( h / rml );
        m11 = ( h / tmb );
        m22 = ( 1.f / fmn );

        // translate
        m30 = ( rpl / lmr );
        m31 = ( tpb / bmt );
        m32 = ( zNear / nmf );

        zpMatrix4f s;
        s.m_m1 = Vector4( m00, z, z, z );
        s.m_m2 = Vector4( z, m11, z, z );
        s.m_m3 = Vector4( z, z, m22, z );
        s.m_m4 = Vector4( m30, m31, m32, o );

        return s;
    }

    ZP_FORCE_INLINE zpMatrix4f ZP_VECTORCALL OrthoRH( zp_float l, zp_float r, zp_float t, zp_float b, zp_float zNear, zp_float zFar )
    {
        zp_float rml, lmr, rpl, tmb, bmt, tpb, nmf, h, z, o;
        zp_float m00, m11, m22, m30, m31, m32;
        h = ( 2.f );
        z = ( 0.f );
        o = ( 1.f );

        rml = ( r - l );
        lmr = ( l - r );
        tmb = ( t - b );
        bmt = ( b - t );
        nmf = ( zNear - zFar );

        rpl = ( r + l );
        tpb = ( t + b );

        // scale
        m00 = ( h / rml );
        m11 = ( h / tmb );
        m22 = ( 1.f / nmf );

        // translate
        m30 = ( rpl / lmr );
        m31 = ( tpb / bmt );
        m32 = ( zNear / nmf );

        zpMatrix4f s;
        s.m_m1 = Vector4( m00, z, z, z );
        s.m_m2 = Vector4( z, m11, z, z );
        s.m_m3 = Vector4( z, z, m22, z );
        s.m_m4 = Vector4( m30, m31, m32, o );

        return s;
    }
};
