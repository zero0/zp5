#pragma once
#ifndef ZP_LZF_H
#define ZP_LZF_H

//
// Original code from: http://www.java2s.com/Code/Java/Collections-Data-Structure/implementstheLZFlosslessdatacompressionalgorithm.htm
//

zp_size_t zp_lzf_compress( const void* inBuff, zp_size_t inPos, zp_size_t inLen, void* outBuff, zp_size_t outPos );
void zp_lzf_expand( const void* inBuff, zp_size_t inPos, zp_size_t inLen, void* outBuff, zp_size_t outPos, zp_size_t outLen );

#endif