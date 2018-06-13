#pragma once
#ifndef ZP_TLSF_H
#define ZP_TLSF_H

//
// Code from: https://github.com/mattconte/tlsf
//

typedef void* zp_tlsf_t;
typedef void* zp_tlsf_pool_t;

zp_size_t zp_tlsf_size();
zp_tlsf_pool_t zp_tlsf_get_pool( zp_tlsf_t tlsf );

zp_tlsf_t zp_tlsf_create( void* memory );
zp_tlsf_t zp_tlsf_create_with_pool( void* memory, zp_size_t size );
void zp_tlsf_destroy( zp_tlsf_t tlsf );

zp_tlsf_pool_t zp_tlsf_add_pool( zp_tlsf_t tlsf, void* memory, zp_size_t size );
void zp_tlsf_remove_pool( zp_tlsf_t tlsf, zp_tlsf_pool_t pool );

void* zp_tlsf_malloc( zp_tlsf_t tlsf, zp_size_t size );
void zp_tlsf_free( zp_tlsf_t tlsf, const void* ptr );

zp_size_t zp_tlsf_block_size( const void* ptr );
zp_size_t zp_tlsf_pool_size( zp_tlsf_pool_t pool );

#endif
