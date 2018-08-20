#pragma once
#ifndef ZP_DATA_BUFFER_H
#define ZP_DATA_BUFFER_H

struct zpDataBufferAllocator
{
    zp_byte* allocate( zp_size_t size );
    void free( zp_byte* ptr );
};

class zpDataBuffer
{
public:
    zpDataBuffer();
    explicit zpDataBuffer( zp_size_t capacity );
    virtual ~zpDataBuffer();

    const zp_byte* getData() const;
    zp_byte* getData();

    const zp_byte* getBuffer() const;
    zp_byte* getBuffer();

    zp_size_t getPosition() const;
    zp_size_t getLength() const;
    zp_size_t getCapacity() const;

    void clear();
    void reset();
    void reserve( zp_size_t capacity );

    zp_size_t write( const void* data, zp_size_t offset, zp_size_t length );
    zp_size_t read( void* data, zp_size_t offset, zp_size_t length );

    template<typename T>
    ZP_FORCE_INLINE zp_size_t write( const T& data )
    {
        return write( &data, 0, sizeof( T ) );
    }

    template<typename T>
    ZP_FORCE_INLINE zp_size_t read( T& data )
    {
        return read( &data, 0, sizeof( T ) );
    }

protected:
    zpDataBuffer( zp_byte* data, zp_size_t capacity );

private:
    void ensureCapacity( zp_size_t capacity );

    zp_byte* m_buffer;
    zp_size_t m_position;
    zp_size_t m_length;
    zp_size_t m_capacity;
    zp_bool m_isFixed;

    zpDataBufferAllocator m_allocator;
};

template< zp_size_t Size >
class zpFixedDataBuffer : public zpDataBuffer
{
    ZP_NON_COPYABLE( zpFixedDataBuffer );
public:
    zpFixedDataBuffer();
    ~zpFixedDataBuffer();

private:
    zp_byte m_byteData[ Size ];
};

template< zp_size_t Size >
zpFixedDataBuffer<Size>::zpFixedDataBuffer()
    : zpDataBuffer( m_byteData, Size )
{
}
template< zp_size_t Size >
zpFixedDataBuffer<Size>::~zpFixedDataBuffer()
{
}

#endif // !ZP_DATA_BUFFER_H
