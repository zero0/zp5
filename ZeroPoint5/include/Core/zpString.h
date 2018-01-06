#pragma once
#ifndef ZP_STRING_H
#define ZP_STRING_H

struct zpStringAllocator
{
    zp_char* allocate( zp_size_t size );
    void free( zp_char* ptr );
};

class zpString
{
public:
    static const zp_size_t npos = -1;

    zpString();
    explicit zpString( const zp_char* );
    zpString( const zp_char*, zp_size_t length );
    zpString( const zpString& other );
    zpString( zpString&& other );
    ~zpString();

    zpString& operator=( const zpString& other );
    zpString& operator=( zpString&& other );
    zpString& operator=( const zp_char* other );

    const zp_char* str() const;

    zp_size_t length() const;
    zp_size_t capacity() const;
    zp_bool isEmpty() const;

    zp_char& operator[]( zp_size_t index );
    const zp_char& operator[]( zp_size_t index ) const;

    zp_char& at( zp_size_t index );
    const zp_char& at( zp_size_t index ) const;

    zp_char& front();
    const zp_char& front() const;

    zp_char& back();
    const zp_char& back() const;

    zp_char* begin();
    const zp_char* begin() const;

    zp_char* end();
    const zp_char* end() const;

    void clear();
    void reserve( zp_size_t size );

    void assign( const zp_char* str);
    void assign( const zp_char* str, zp_size_t length );

    zpString substr( zp_size_t pos, zp_size_t count = npos ) const;

private:
    zp_char* m_str;
    zp_size_t m_length;
    zp_size_t m_capacity;
    zpStringAllocator m_alloc;
};

#endif // !ZP_STRING_H
