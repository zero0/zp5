#include "Rendering/zpRendering.h"

const zpRenderTargetBlendState zpRenderTargetBlendState::Default = {
    ZP_COLOR_MASK_ALL,
    ZP_BLEND_MODE_ONE,
    ZP_BLEND_MODE_ZERO,
    ZP_BLEND_MODE_ONE,
    ZP_BLEND_MODE_ZERO,
    ZP_BLEND_OPERATION_ADD,
    ZP_BLEND_OPERATION_ADD
};

const zpBlendState zpBlendState::Default = {
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    zpRenderTargetBlendState::Default,
    false,
    false,
    false
};

const zpDepthState zpDepthState::Default = {
    ZP_COMPARE_FUNCTION_ALWAYS,
    true
};

const zpStencilState zpStencilState::Default = {
    ZP_COMPARE_FUNCTION_ALWAYS,
    ZP_STENCIL_OPERATION_KEEP,
    ZP_STENCIL_OPERATION_KEEP,
    ZP_STENCIL_OPERATION_KEEP,
    ZP_COMPARE_FUNCTION_ALWAYS,
    ZP_STENCIL_OPERATION_KEEP,
    ZP_STENCIL_OPERATION_KEEP,
    ZP_STENCIL_OPERATION_KEEP,
    0xFF,
    0xFF,
    false
};

const zpRasterState zpRasterState::Default = {
    ZP_FILL_MODE_FILL,
    ZP_CULL_MODE_BACK,
    0.f,
    0,
    false
};
