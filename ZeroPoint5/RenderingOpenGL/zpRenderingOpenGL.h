#pragma once
#ifndef ZP_RENDERING_OPENGL_H
#define ZP_RENDERING_OPENGL_H

#pragma comment( lib, "opengl32.lib" )

#include "Rendering\zpRendering.h"

void SetupRenderingOpenGL( zp_handle hWindow, zp_handle& hDC, zp_handle& hContext );
void TeardownRenderingOpenGL( zp_handle hContext );
void ProcessRenderingCommandOpenGL( zpRenderingCommand* cmd );
void PresentOpenGL( zp_handle hDC, zp_handle hContext );

void CreateRenderBufferOpenGL( const void* data, zp_size_t size, zpBufferType type, zpBufferBindType bindType, zpRenderBuffer& buffer );
void DestroyRenderBufferOpenGL( zpRenderBuffer& buffer );
void SetRenderBufferDataOpenGL( const zpRenderBuffer& buffer, const void* data, zp_size_t offset, zp_size_t length );

void CreateTextureOpenGL( zpTexture& texture );
void DestroyTextureOpenGL( zpTexture& texture );

#endif // !ZP_RENDERING_OPENGL_H
