#pragma once
#ifndef ZP_FILE_H
#define ZP_FILE_H

enum zpFileResult : zp_int
{
    ZP_FILE_SUCCESS = 0,

    ZP_FILE_ERROR_ON_OPEN = -1,
    ZP_FILE_ERROR_ON_CLOSE = -2,

    zpFileResult_Force32 = ZP_FORECE_32BIT

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

    zpFileMode_Count,
    zpFileMode_Force32 = ZP_FORECE_32BIT
};

class zpFile
{
public:
    static const zp_size_t eof = -1;

    zpFile( const zp_char* filename, zpFileMode filemode );
    ~zpFile();

    zpFileResult getResult() const;

    zp_size_t write( const void* data, zp_size_t offset, zp_size_t length ) const;
    zp_size_t read( void* data, zp_size_t offset, zp_size_t length ) const;

    static zp_time_t lastModifiedTime( const zp_char* filepath );
    static zp_time_t lastAccessedTime( const zp_char* filepath );
    static zp_time_t creationTime( const zp_char* filepath );
    static zp_bool exists( const zp_char* filepath );
    static zp_size_t size( const zp_char* filepath );

private:
    zpFile();

    zpString m_filename;
    zp_handle m_fileHandle;
    zpFileResult m_result;
};

#endif // !ZP_FILE_H
