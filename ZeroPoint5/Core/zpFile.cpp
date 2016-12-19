#include "zpCore.h"
#include <stdio.h>

const zp_char* g_zpFileModeToString[] =
{
    "",
    "r",
    "a",
    "w",

    "r+",
    "a+",
    "w+",

    "rb",
    "ab",
    "wb",

    "rb+",
    "ab+",
    "wb+"
};

zpFile::zpFile()
    : m_filename()
    , m_fileHandle( ZP_NULL )
{}

zpFile::zpFile( const zp_char* filename )
    : m_filename( filename )
    , m_fileHandle( ZP_NULL )
{
}

zpFile::~zpFile()
{
}

zpFileResult zpFile::open( zpFileMode filemode )
{
    FILE* f;
#ifdef ZP_USE_SAFE_FUNCTIONS
    fopen_s( &f, m_filename.str(), g_zpFileModeToString[ filemode ] );
#else
    f = fopen( m_filename.str(), g_zpFileModeToString[ filemode ] );
#endif

    m_fileHandle = f;

    zpFileResult result = ZP_FILE_SUCCESS;
    return result;
}

zpFileResult zpFile::close()
{
    FILE* f = static_cast<FILE*>( m_fileHandle );
    zp_int r = fclose( f );

    m_fileHandle = ZP_NULL;

    zpFileResult result = r == 0 ? ZP_FILE_SUCCESS : ZP_FILE_ERROR_ON_CLOSE;
    return result;
}

zp_size_t zpFile::write( const void* data, zp_size_t offset, zp_size_t length ) const
{
    FILE* f = static_cast<FILE*>( m_fileHandle );
    const zp_byte* d = static_cast<const zp_byte*>( data );

    zp_size_t r = fwrite( d + offset, sizeof( zp_byte ), length, f );
    return r;
}

zp_size_t zpFile::read( void* data, zp_size_t offset, zp_size_t length ) const
{
    FILE* f = static_cast<FILE*>( m_fileHandle );
    zp_byte* d = static_cast<zp_byte*>( data );

    zp_size_t r;
#ifdef ZP_USE_SAFE_FUNCTIONS
    r = fread_s( d + offset, length, sizeof( zp_byte ), length, f );
#else
    r = fread( d + offset, sizeof( zp_byte ), length, f );
#endif

    return r;
}
