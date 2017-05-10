
set( ZP_VERSION_MAJOR 0 )
set( ZP_VERSION_MINOR 0 )
set( ZP_VERSION_PATCH 1 )
set( ZP_VERSION ${ZP_VERSION_MAJOR}.${ZP_VERSION_MINOR}.${ZP_VERSION_PATCH} )

project( ZeroPoint5 VERSION ${ZP_VERSION} LANGUAGES C CXX )

option( ZP_INSTALL "Install ZeroPoint5" OFF )
option( ZP_BUILD_SHARED "Build ZeroPoint5 as shared library" OFF )
option( ZP_BUILD_STATIC "Build ZeroPoint5 as static library" ON )
option( ZP_BUILD_SINGLE "Build ZeroPoint5 as single library" ON )

option( ZP_USE_HOT_RELOAD "Build ZeroPoint5 with hot reload" ON )
option( ZP_USE_ASSERTIONS "Build ZeroPoint5 with assersions" ON )
option( ZP_USE_PRINT "Build ZeroPoint5 with prints" ON )
option( ZP_USE_PROFILER "Build ZeroPoint5 with profiler" ON )
option( ZP_USE_SAFE_FUNCTIONS "Build ZeroPoint5 with safe functions" ON )
option( ZP_USE_SIMD "Build ZeroPoint5 with SIMD" ON )
option( ZP_USE_OPENGL_RENDERING "Build ZeroPoint5 with OpenGL Rendering" ON )
option( ZP_USE_D3D_RENDERING "Build ZeroPoint5 with D3D Rendering" OFF )

include_directories( ${PROJECT_SOURCE_DIR} )

# Core Config
configure_file(
    "${PROJECT_SOURCE_DIR}/Core/zpConfig.h.in"
    "${PROJECT_BINARY_DIR}/Core/zpConfig.h"
)

file( GLOB ZP_CORE_SOURCE "Core/*.cpp" )
file( GLOB ZP_CORE_HEADER "Core/*.h" )
file( GLOB ZP_CORE_INLINE "Core/*.inl" )

source_group( Core\\Headers FILES
    ${ZP_CORE_HEADER}
)
source_group( Core\\Inline FILES
    ${ZP_CORE_INLINE}
)
source_group( Core\\Source FILES
    ${ZP_CORE_SOURCE}
)

# Engine Config
file( GLOB ZP_ENGINE_SOURCE "Engine/*.cpp" )
file( GLOB ZP_ENGINE_HEADER "Engine/*.h" )
file( GLOB ZP_ENGINE_INLINE "Engine/*.inl" )

source_group( Engine\\Headers FILES
    ${ZP_ENGINE_HEADER}
)
source_group( Engine\\Inline FILES
    ${ZP_ENGINE_INLINE}
)
source_group( Engine\\Source FILES
    ${ZP_ENGINE_SOURCE}
)

# Rendering Config
file( GLOB ZP_RENDERING_SOURCE "Rendering/*.cpp" )
file( GLOB ZP_RENDERING_HEADER "Rendering/*.h" )
file( GLOB ZP_RENDERING_INLINE "Rendering/*.inl" )

source_group( Rendering\\Headers FILES
    ${ZP_RENDERING_HEADER}
)
source_group( Rendering\\Source FILES
    ${ZP_RENDERING_INLINE}
    ${ZP_RENDERING_SOURCE}
)

# RenderingOpenGL Config
file( GLOB ZP_RENDERING_OPENGL_SOURCE "RenderingOpenGL/*.cpp" )
file( GLOB ZP_RENDERING_OPENGL_HEADER "RenderingOpenGL/*.h" )
file( GLOB ZP_RENDERING_OPENGL_INLINE "RenderingOpenGL/*.inl" )

source_group( RenderingOpenGL\\Headers FILES
    ${ZP_RENDERING_OPENGL_HEADER}
)
source_group( RenderingOpenGL\\Source FILES
    ${ZP_RENDERING_OPENGL_INLINE}
    ${ZP_RENDERING_OPENGL_SOURCE}
)

# GLEW
file( GLOB ZP_RENDERING_GLEW_SOURCE "src/*.c" )
file( GLOB ZP_RENDERING_GLEW_HEADER "include/GL/*.h" )
include_directories( "include" )

source_group( GLEW\\Headers FILES
    ${ZP_RENDERING_GLEW_HEADER}
)
source_group( GLEW\\Source FILES
    ${ZP_RENDERING_GLEW_SOURCE}
)

# RenderingD3D Config
file( GLOB ZP_RENDERING_D3D_SOURCE "RenderingD3D/*.cpp" )
file( GLOB ZP_RENDERING_D3D_HEADER "RenderingD3D/*.h" )
file( GLOB ZP_RENDERING_D3D_INLINE "RenderingD3D/*.inl" )

source_group( RenderingD3D\\Headers FILES
    ${ZP_RENDERING_D3D_HEADER}
)
source_group( RenderingD3D\\Source FILES
    ${ZP_RENDERING_D3D_INLINE}
    ${ZP_RENDERING_D3D_SOURCE}
)

# Rendering Impl OpenGL Config
if( ZP_USE_OPENGL_RENDERING )
    set( ZP_RENDERING_IMPL_SOURCE ${ZP_RENDERING_OPENGL_SOURCE} ${ZP_RENDERING_GLEW_SOURCE} )
    set( ZP_RENDERING_IMPL_HEADER ${ZP_RENDERING_OPENGL_HEADER} ${ZP_RENDERING_GLEW_HEADER} )
    set( ZP_RENDERING_IMPL_INLINE ${ZP_RENDERING_OPENGL_INLINE} )
endif( ZP_USE_OPENGL_RENDERING )

# Rendering Impl D3D Config
if( ZP_USE_D3D_RENDERING )
    set( ZP_RENDERING_IMPL_SOURCE ${ZP_RENDERING_D3D_SOURCE} )
    set( ZP_RENDERING_IMPL_HEADER ${ZP_RENDERING_D3D_HEADER} )
    set( ZP_RENDERING_IMPL_INLINE ${ZP_RENDERING_D3D_INLINE} )
endif( ZP_USE_D3D_RENDERING )

# All Config
set( ZP_ALL_HEADER
    ${ZP_CORE_HEADER}
    ${ZP_ENGINE_HEADER}
    ${ZP_RENDERING_HEADER}
    ${ZP_RENDERING_IMPL_HEADER}
)
set( ZP_ALL_INLINE
    ${ZP_CORE_INLINE}
    ${ZP_ENGINE_INLINE}
    ${ZP_RENDERING_INLINE}
    ${ZP_RENDERING_IMPL_INLINE}
)
set( ZP_ALL_SOURCE
    ${ZP_CORE_SOURCE}
    ${ZP_ENGINE_SOURCE}
    ${ZP_RENDERING_SOURCE}
    ${ZP_RENDERING_IMPL_SOURCE}
)

# Static Lib build
if( ZP_BUILD_STATIC )
    if( ZP_BUILD_SINGLE )
        add_library( ZeroPoint5_lib STATIC
            ${ZP_ALL_HEADER}
            ${ZP_ALL_SOURCE}
        )

        set_property( TARGET ZeroPoint5_lib PROPERTY FOLDER "Libraries" )
        set_target_properties( ZeroPoint5_lib PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            CLEAN_DIRECT_OUTPUT 1
            VERSION ${ZP_VERSION}
        )
    endif( ZP_BUILD_SINGLE )

    # Core
    add_library( ZeroPoint5_Core_lib STATIC
        ${ZP_CORE_HEADER}
        ${ZP_CORE_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Core_lib PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Core_lib PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Core
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )
endif( ZP_BUILD_STATIC )

# Shared DLL build
if( ZP_BUILD_SHARED )
    if( ZP_BUILD_SINGLE )
        add_library( ZeroPoint5_dll SHARED
            ${ZP_ALL_HEADER}
            ${ZP_ALL_SOURCE}
        )

        set_property( TARGET ZeroPoint5_dll PROPERTY FOLDER "Libraries" )
        set_target_properties( ZeroPoint5_dll PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}
            CLEAN_DIRECT_OUTPUT 1
            VERSION ${ZP_VERSION}
        )
    endif( ZP_BUILD_SINGLE )

    # Core
    add_library( ZeroPoint5_Core_dll SHARED
        ${ZP_CORE_HEADER}
        ${ZP_CORE_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Core_dll PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Core_dll PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Core
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )
endif( ZP_BUILD_SHARED )

# Install
if( ZP_INSTALL )
    install( FILES ${ZP_CORE_HEADER} ${ZP_CORE_INLINE} DESTINATION include/Core )
    install( FILES ${ZP_ENGINE_HEADER} ${ZP_ENGINE_INLINE} DESTINATION include/Engine )

    install( TARGETS ZeroPoint5_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    install( TARGETS ZeroPoint5_dll
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif( ZP_INSTALL )
