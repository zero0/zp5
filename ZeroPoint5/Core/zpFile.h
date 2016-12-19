#pragma once
#ifndef ZP_FILE_H
#define ZP_FILE_H

struct zpFileAllocator
{
    zp_char* allocate( zp_size_t size );
    void free( zp_char* ptr );
};

enum zpFileResult : zp_uint
{
    ZP_FILE_SUCCESS = 0,

    ZP_FILE_ERROR_NOT_FOUND = (zp_uint)-1,
    ZP_FILE_ERROR_ON_CLOSE = (zp_uint)-2,
};

enum zpFileMode : zp_uint
{
    ZP_FILE_MODE_NONE,

    ZP_FILE_MODE_ASCII_READ,
    ZP_FILE_MODE_ASCII_APPEND_WRITE,
    ZP_FILE_MODE_ASCII_TRUNCATE_WRITE,

    ZP_FILE_MODE_ASCII_READ_WRITE,
    ZP_FILE_MODE_ASCII_READ_APPEND_WRITE,
    ZP_FILE_MODE_ASCII_READ_TRUNCATE_WRITE,

    ZP_FILE_MODE_BINARY_READ,
    ZP_FILE_MODE_BINARY_APPEND_WRITE,
    ZP_FILE_MODE_BINARY_TRUNCATE_WRITE,

    ZP_FILE_MODE_BINARY_READ_WRITE,
    ZP_FILE_MODE_BINARY_READ_APPEND_WRITE,
    ZP_FILE_MODE_BINARY_READ_TRUNCATE_WRITE,
};

class zpFile
{
public:
    static const zp_size_t eof = -1;

    explicit zpFile( const zp_char* filename );
    ~zpFile();

    zpFileResult open( zpFileMode filemode );
    zpFileResult close();

    zp_size_t write( const void* data, zp_size_t offset, zp_size_t length ) const;
    zp_size_t read( void* data, zp_size_t offset, zp_size_t length ) const;

private:
    zpFile();

    zpString m_filename;

    zp_handle m_fileHandle;
};

#endif // !ZP_FILE_H
