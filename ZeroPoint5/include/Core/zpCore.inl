
template<typename T>
ZP_FORCE_INLINE void zp_zero_memory( T* ptr )
{
    zp_memset( ptr, 0, sizeof( T ) );
}

template<typename T>
ZP_FORCE_INLINE void zp_zero_memory_array( T* ptr, zp_size_t count )
{
    zp_memset( ptr, 0, count * sizeof( T ) );
}

template<typename T, zp_size_t Size>
ZP_FORCE_INLINE void zp_zero_memory_array( T( &arr )[ Size ] )
{
    zp_memset( arr, 0, Size * sizeof( T ) );
}

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
    b = zp_move( zp_forward( t ) );
}

template<typename T>
ZP_FORCE_INLINE void zp_move_swap( T&& a, T&& b )
{
    T t = zp_move( a );
    a = zp_move( b );
    b = zp_move( zp_forward( t ) );
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

template<typename T, typename Cmp>
void zp_qsort3( T* begin, T* end, Cmp cmp )
{
    if( begin >= end ) return;

    T* i;
    T* j;

    // partition
    T* mid = begin;
    T* pivot = end;

    while( mid <= end )
    {
        zp_int c = cmp( *mid, *pivot );
        if( c < 0 )
        {
            zp_move_swap( zp_forward( *begin ), zp_forward( *mid ) );
            ++begin;
            ++mid;
        }
        else if( c > 0 )
        {
            zp_move_swap( zp_forward( *mid ), zp_forward( *end ) );
            --end;
        }
        else
        {
            ++mid;
        }
    }

    i = begin - 1;
    j = mid;

    zp_qsort3( begin, i, cmp );
    zp_qsort3( j, end, cmp );
}