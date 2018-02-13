#include "Core/zpCore.h"

enum zp_lzf_config
{
    /**
     * The number of entries in the hash table. The size is a trade-off between
     * hash collisions (reduced compression) and speed (amount that fits in CPU
     * cache).
     */
    HASH_SIZE = 1 << 14,

    //HASH_CONST = 2777,
    HASH_CONST = 57321, // better compression hash

    /**
     * The maximum number of literals in a chunk (32).
     */
    MAX_LITERAL = 1 << 5,

    /**
     * The maximum offset allowed for a back-reference (8192).
     */
    MAX_OFF = 1 << 13,

    /**
     * The maximum back-reference length (264).
     */
    MAX_REF = ( 1 << 8 ) + ( 1 << 3 ),
};

/**
 * Return byte with lower 2 bytes being byte at index, then index+1.
 */
ZP_FORCE_INLINE zp_int zp_lzf_first( const zp_byte* inBuff, zp_size_t inPos )
{
    return ( inBuff[ inPos ] << 8 ) | ( inBuff[ inPos + 1 ] & 0xFF );
}

/**
 * Shift v 1 byte left, add value at index inPos+2.
 */
ZP_FORCE_INLINE zp_int zp_lzf_next( zp_byte v, const zp_byte* inBuff, zp_size_t inPos )
{
    return ( v << 8 ) | ( inBuff[ inPos + 2 ] & 0xFF );
}

/**
 * Compute the address in the hash table.
 */
ZP_FORCE_INLINE zp_size_t zp_lzf_hash( zp_int h )
{
    return ( ( h * HASH_CONST ) >> 9 ) & ( HASH_SIZE - 1 );
}

zp_size_t zp_lzf_compress( const void* inBuffer, zp_size_t inPos, zp_size_t inLen, void* outBuffer, zp_size_t outPos )
{
    zp_size_t hashTab[ HASH_SIZE ];

    const zp_byte* inBuff = static_cast<const zp_byte*>( inBuffer );
    zp_byte* outBuff = static_cast<zp_byte*>( outBuffer );

    zp_int literals = 0;

    outPos++;

    zp_int future = zp_lzf_first( inBuff, inPos );
    zp_size_t off;
    zp_size_t reff;
    zp_size_t maxLen;
    zp_int len;

    while( inPos < inLen - 4 )
    {
        zp_byte p2 = inBuff[ inPos + 2 ];
        // next
        future = ( future << 8 ) + ( p2 & 0xFF );
        off = zp_lzf_hash( future );
        reff = hashTab[ off ];
        hashTab[ off ] = inPos;
        if( reff < inPos
            && reff > 0
            && ( off = inPos - reff - 1 ) < MAX_OFF
            && inBuff[ reff + 2 ] == p2
            && inBuff[ reff + 1 ] == static_cast<zp_byte>( future >> 8 )
            && inBuff[ reff ] == static_cast<zp_byte>( future >> 16 ) )
        {
            // match
            maxLen = inLen - inPos - 2;
            if( maxLen > MAX_REF )
            {
                maxLen = MAX_REF;
            }

            if( literals == 0 )
            {
                // multiple back-references,
                // so there is no literal run control byte
                outPos--;
            }
            else
            {
                // set the control byte at the start of the literal run
                // to store the number of literals
                outBuff[ outPos - literals - 1 ] = static_cast<zp_byte>( literals - 1 );
                literals = 0;
            }

            len = 3;
            while( len < maxLen && inBuff[ reff + len ] == inBuff[ inPos + len ] )
            {
                len++;
            }

            len -= 2;
            if( len < 7 )
            {
                outBuff[ outPos++ ] = static_cast<zp_byte>( ( off >> 8 ) + ( len << 5 ) );
            }
            else
            {
                outBuff[ outPos++ ] = static_cast<zp_byte>( ( off >> 8 ) + ( 7 << 5 ) );
                outBuff[ outPos++ ] = static_cast<zp_byte>( len - 7 );
            }

            outBuff[ outPos++ ] = (zp_byte)off;
            // move one byte forward to allow for a literal run control byte
            outPos++;
            inPos += len;
            // Rebuild the future, and store the last bytes to the hashtable.
            // Storing hashes of the last bytes in back-reference improves
            // the compression ratio and only reduces speed slightly.
            future = zp_lzf_first( inBuff, inPos );
            future = zp_lzf_next( future, inBuff, inPos );
            hashTab[ zp_lzf_hash( future ) ] = inPos++;
            future = zp_lzf_next( future, inBuff, inPos );
            hashTab[ zp_lzf_hash( future ) ] = inPos++;
        }
        else
        {
            // copy one byte from input to output as part of literal
            outBuff[ outPos++ ] = inBuff[ inPos++ ];
            literals++;
            // at the end of this literal chunk, write the length
            // to the control byte and start a new chunk
            if( literals == MAX_LITERAL )
            {
                outBuff[ outPos - literals - 1 ] = static_cast<zp_byte>( literals - 1 );
                literals = 0;
                // move ahead one byte to allow for the
                // literal run control byte
                outPos++;
            }
        }
    }

    // write the remaining few bytes as literals
    while( inPos < inLen )
    {
        outBuff[ outPos++ ] = inBuff[ inPos++ ];
        literals++;
        if( literals == MAX_LITERAL )
        {
            outBuff[ outPos - literals - 1 ] = static_cast<zp_byte>( literals - 1 );
            literals = 0;
            outPos++;
        }
    }

    // writes the final literal run length to the control byte
    outBuff[ outPos - literals - 1 ] = static_cast<zp_byte>( literals - 1 );
    if( literals == 0 )
    {
        outPos--;
    }

    return outPos;
}

void zp_lzf_expand( const void* inBuffer, zp_size_t inPos, zp_size_t inLen, void* outBuffer, zp_size_t outPos, zp_size_t outLen )
{
    zp_ptrdiff_t ctrl;
    zp_size_t len;
    zp_size_t i;

    const zp_byte* inBuff = static_cast<const zp_byte*>( inBuffer );
    zp_byte* outBuff = static_cast<zp_byte*>( outBuffer );

    do
    {
        ctrl = inBuff[ inPos++ ] & 0xFF;
        if( ctrl < MAX_LITERAL )
        {
            // literal run of length = ctrl + 1,
            ctrl++;
            // copy to output and move forward this many bytes
            for( i = 0; i != ctrl; i++ )
            {
                outBuff[ outPos++ ] = inBuff[ inPos++ ];
            }
            //System.Buffer.BlockCopy( inBuff, inPos, outBuff, outPos, ctrl );
            //System.Array.Copy( inBuff, inPos, outBuff, outPos, ctrl );
            //outPos += ctrl;
            //inPos += ctrl;
        }
        else
        {
            // back reference
            // the highest 3 bits are the match length
            len = ctrl >> 5;
            // if the length is maxed, add the next byte to the length
            if( len == 7 )
            {
                len += inBuff[ inPos++ ] & 0xFF;
            }
            // minimum back-reference is 3 bytes,
            // so 2 was subtracted before storing size
            len += 2;

            // ctrl is now the offset for a back-reference...
            // the logical AND operation removes the length bits
            ctrl = -( ( ctrl & 0x1f ) << 8 ) - 1;

            // the next byte augments/increases the offset
            ctrl -= inBuff[ inPos++ ] & 0xFF;

            // copy the back-reference bytes from the given
            // location in output to current position
            ctrl += outPos;
            if( outPos + len >= outLen )
            {
                // reduce array bounds checking
                break;
            }

            for( i = 0; i != len; i++ )
            {
                outBuff[ outPos++ ] = outBuff[ ctrl++ ];
            }
            //System.Buffer.BlockCopy( outBuff, ctrl, outBuff, outPos, len );
            //System.Array.Copy( outBuff, ctrl, outBuff, outPos, len );
            //outPos += len;
            //ctrl += len;
        }
    } while ( outPos < outLen );
}
