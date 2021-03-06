#include "Core/zpCore.h"
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

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

ZP_STATIC_ASSERT( ( sizeof( g_zpFileModeToString ) / sizeof( g_zpFileModeToString[ 0 ] ) ) == zpFileMode_Count );
    
zpFile::zpFile( const zp_char* filename, zpFileMode filemode )
    : m_filename( filename )
    , m_fileHandle( ZP_NULL )
{
    FILE* f;
    errno_t err;
#ifdef ZP_USE_SAFE_FUNCTIONS
    err = fopen_s( &f, m_filename.str(), g_zpFileModeToString[ filemode ] );
#else
    f = fopen( m_filename.str(), g_zpFileModeToString[ filemode ] );
    err = errno;
#endif
    m_fileHandle = f;

    m_result = err == 0 ? ZP_FILE_SUCCESS : ZP_FILE_ERROR_ON_OPEN;
}

zpFile::~zpFile()
{
    if( m_fileHandle )
    {
        FILE* f = static_cast<FILE*>( m_fileHandle );
        errno_t err = fclose( f );

        m_fileHandle = ZP_NULL;
        m_result = err == 0 ? ZP_FILE_SUCCESS : ZP_FILE_ERROR_ON_CLOSE;
    }
}

zpFileResult zpFile::getResult() const
{
    return m_result;
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

zp_time_t zpFile::lastModifiedTime( const zp_char* filepath )
{
    zp_time_t lastModifiedTime = 0;

    struct stat fileStat;
    zp_int r = stat( filepath, &fileStat );
    lastModifiedTime = r == 0 ? static_cast<zp_time_t>( fileStat.st_mtime ) : 0;

    return lastModifiedTime;
}

zp_time_t zpFile::lastAccessedTime( const zp_char* filepath )
{
    zp_time_t lastAccessedTime = 0;

    struct stat fileStat;
    zp_int r = stat( filepath, &fileStat );
    lastAccessedTime = r == 0 ? static_cast<zp_time_t>( fileStat.st_atime ) : 0;

    return lastAccessedTime;
}

zp_time_t zpFile::creationTime( const zp_char* filepath )
{
    zp_time_t creationTime = 0;

    struct stat fileStat;
    zp_int r = stat( filepath, &fileStat );
    creationTime = r == 0 ? static_cast<zp_time_t>( fileStat.st_ctime ) : 0;

    return creationTime;
}

zp_bool zpFile::exists( const zp_char* filepath )
{
    zp_bool exists = false;

    struct stat fileStat;
    zp_int r = stat( filepath, &fileStat );
    exists = r == 0;

    return exists;
}

zp_size_t zpFile::size( const zp_char* filepath )
{
    zp_size_t size = 0;

    struct stat fileStat;
    zp_int r = stat( filepath, &fileStat );
    size = r == 0 ? static_cast<zp_size_t>( fileStat.st_size ) : 0;

    return size;
}
