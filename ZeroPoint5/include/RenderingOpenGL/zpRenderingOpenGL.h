#pragma once
#ifndef ZP_RENDERING_OPENGL_H
#define ZP_RENDERING_OPENGL_H

#include "Rendering\zpRendering.h"

void SetupRenderingOpenGL( zp_handle hWindow, zp_handle& hDC, zp_handle& hContext );
void TeardownRenderingOpenGL( zp_handle hContext );

void ProcessRenderingCommandOpenGL( const zpRenderingCommand* cmd );
void ProcessRenderCommandOpenGL( const void* cmd, zp_size_t size );
void PresentOpenGL( zp_handle hDC, zp_handle hContext );

void CreateRenderBufferOpenGL( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer );
void DestroyRenderBufferOpenGL( zpRenderBuffer& buffer );
void SetRenderBufferDataOpenGL( const zpRenderBuffer& buffer, const void* data, zp_size_t offset, zp_size_t length );

void CreateTextureOpenGL( zpCreateTextureDesc* desc, zpTexture& texture );
void DestroyTextureOpenGL( zpTexture& texture );

void CreateShaderOpenGL( const zp_char* shaderName, const zp_char* vertexShaderSource, const zp_char* fragmentShaderSource, zpShader& shader );
void DestroyShaderOpenGL( zpShader& shader );

void BeginFrameOpenGL( zp_size_t frameIndex );
void EndFrameOpenGL( zp_size_t frameIndex, zpRenderingStat& stat );

#endif // !ZP_RENDERING_OPENGL_H
