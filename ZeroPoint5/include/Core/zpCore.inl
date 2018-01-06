
template<typename T>
ZP_FORCE_INLINE void zp_swap( T& a, T& b )
{
    T t = a;
    a = b;
    b = t;
}

template<typename T>
ZP_FORCE_INLINE void zp_move_swap( T& a, T& b )
{
    T t = zp_move( zp_forward( a ) );
    a = zp_move( zp_forward( b ) );
    b = zp_move( t );
}

template<typename T>
ZP_FORCE_INLINE void zp_move_swap( T&& a, T&& b )
{
    T t = zp_move( a );
    a = zp_move( b );
    b = zp_move( t );
}

template<typename T>
ZP_FORCE_INLINE ZP_CONSTEXPR T&& zp_forward( T& a )
{
    return static_cast<T&&>( a );
}

template<typename T>
ZP_FORCE_INLINE ZP_CONSTEXPR T&& zp_forward( T&& a )
{
    return static_cast<T&&>( a );
}

template<typename T>
ZP_FORCE_INLINE ZP_CONSTEXPR T&& zp_move( T&& a )
{
    return static_cast<T&&>( a );
}

template<typename T, typename Cmp>
void zp_qsort( T* begin, T* end, Cmp cmp )
{
    if( begin >= end ) return;

    zp_size_t mid = ( end - begin ) / 2;
    T* pivot = begin + mid;

    zp_move_swap( *begin, *pivot );
    T* low = begin + 1;
    T* high = end;

    while( low < high )
    {
        if( cmp( *low, *begin ) )
        {
            ++low;
        }
        else if( cmp( *( --high ), *begin ) )
        {
            zp_move_swap( *low, *high );
        }
    }

    zp_move_swap( *begin, *( --low ) );
    pivot = low;

    if( begin < pivot ) zp_qsort( begin, pivot, cmp );
    if( pivot < end ) zp_qsort( pivot + 1, end, cmp );
}
