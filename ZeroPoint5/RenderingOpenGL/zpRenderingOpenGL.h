#pragma once
#ifndef ZP_RENDERING_OPENGL_H
#define ZP_RENDERING_OPENGL_H

#pragma comment( lib, "opengl32.lib" )

#include "Rendering\zpRendering.h"

void SetupRenderingOpenGL( zp_handle hWindow, zp_handle& hDC, zp_handle& hContext );
void TeardownRenderingOpenGL( zp_handle hContext );
void ProcessRenderingCommandOpenGL( zpRenderingCommand* cmd );
void PresentOpenGL( zp_handle hDC, zp_handle hContext );

#endif // !ZP_RENDERING_OPENGL_H
