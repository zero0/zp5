
set( ZP_VERSION_MAJOR 0 )
set( ZP_VERSION_MINOR 0 )
set( ZP_VERSION_PATCH 1 )
set( ZP_VERSION ${ZP_VERSION_MAJOR}.${ZP_VERSION_MINOR}.${ZP_VERSION_PATCH} )

project( ZeroPoint5 VERSION ${ZP_VERSION} LANGUAGES C CXX )

option( ZP_INSTALL "Install ZeroPoint5" OFF )
option( ZP_BUILD_SHARED "Build ZeroPoint5 as shared library" OFF )
option( ZP_BUILD_STATIC "Build ZeroPoint5 as static library" ON )
option( ZP_BUILD_SINGLE "Build ZeroPoint5 as single library" OFF )

option( ZP_USE_HOT_RELOAD "Build ZeroPoint5 with hot reload" ON )
option( ZP_USE_ASSERTIONS "Build ZeroPoint5 with assersions" ON )
option( ZP_USE_PRINT "Build ZeroPoint5 with prints" ON )
option( ZP_USE_PROFILER "Build ZeroPoint5 with profiler" ON )
option( ZP_USE_SAFE_FUNCTIONS "Build ZeroPoint5 with safe functions" ON )
option( ZP_USE_SIMD "Build ZeroPoint5 with SIMD" ON )
option( ZP_USE_OPENGL_RENDERING "Build ZeroPoint5 with OpenGL Rendering" ON )
option( ZP_USE_D3D_RENDERING "Build ZeroPoint5 with D3D Rendering" OFF )

include_directories( "include" )
if( WIN32 )
include_directories( BEFORE SYSTEM
    "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/ucrt"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/um"
)
endif()

# Core Config
configure_file(
    "include/Core/zpConfig.h.in"
    "include/Core/zpConfig.h"
)

set( ZP_CORE_SOURCE
    "src/Core/zpArchive.cpp"
    "src/Core/zpCore.cpp"
    "src/Core/zpDataBuffer.cpp"
    "src/Core/zpFile.cpp"
    "src/Core/zpLZF.cpp"
    "src/Core/zpMath.cpp"
    "src/Core/zpMemoryAllocator.cpp"
    "src/Core/zpMemoryStorage.cpp"
    "src/Core/zpMemoryPolicy.cpp"
    "src/Core/zpProfiler.cpp"
    "src/Core/zpStackTrace.cpp"
    "src/Core/zpString.cpp"
    "src/Core/zpTime.cpp"
    "src/Core/zpTLSF.cpp"
    )
set( ZP_CORE_HEADER
    "include/Core/zpArchive.h"
    "include/Core/zpConfig.h"
    "include/Core/zpCore.h"
    "include/Core/zpDataBuffer.h"
    "include/Core/zpFile.h"
    "include/Core/zpLZF.h"
    "include/Core/zpMap.h"
    "include/Core/zpMath.h"
    "include/Core/zpMemoryAllocator.h"
    "include/Core/zpMemoryStorage.h"
    "include/Core/zpMemoryPolicy.h"
    "include/Core/zpProfiler.h"
    "include/Core/zpQueue.h"
    "include/Core/zpStack.h"
    "include/Core/zpStackTrace.h"
    "include/Core/zpString.h"
    "include/Core/zpTime.h"
    "include/Core/zpTLSF.h"
    "include/Core/zpVector.h"
    )
set( ZP_CORE_INLINE
    "include/Core/zpCore.inl"
    "include/Core/zpMathSimd.inl"
    "include/Core/zpVector.inl"
    )

source_group( Headers FILES
    ${ZP_CORE_INLINE}
    ${ZP_CORE_HEADER}
)
source_group( Source FILES
    ${ZP_CORE_SOURCE}
)

# Engine Config
set( ZP_ENGINE_SOURCE
    "src/Engine/zpBaseApplication.cpp"
    "src/Engine/zpDebug.cpp"
    "src/Engine/zpInput.cpp"
    "src/Engine/zpMain.cpp"
    "src/Engine/zpMeshRendererComponent.cpp"
    "src/Engine/zpObject.cpp"
    "src/Engine/zpObjectManager.cpp"
    "src/Engine/zpParticleEmitterComponent.cpp"
    "src/Engine/zpScene.cpp"
    "src/Engine/zpTransformComponent.cpp"
    "src/Engine/zpRectTransformComponent.cpp" )
set( ZP_ENGINE_HEADER
    "include/Engine/zpAllComponents.h"
    "include/Engine/zpBaseApplication.h"
    "include/Engine/zpComponent.h"
    "include/Engine/zpDebug.h"
    "include/Engine/zpEngine.h"
    "include/Engine/zpHandle.h"
    "include/Engine/zpInput.h"
    "include/Engine/zpMain.h"
    "include/Engine/zpMeshRendererComponent.h"
    "include/Engine/zpObject.h"
    "include/Engine/zpObjectManager.h"
    "include/Engine/zpParticleEmitterComponent.h"
    "include/Engine/zpScene.h"
    "include/Engine/zpTransformComponent.h"
    "include/Engine/zpRectTransformComponent.h" )

source_group( Headers FILES
    ${ZP_ENGINE_HEADER}
)
source_group( Source FILES
    ${ZP_ENGINE_SOURCE}
)

# Audio Config
set( ZP_AUDIO_SOURCE
    "src/Audio/zpAudio.cpp" )
set( ZP_AUDIO_HEADER
    "include/Audio/zpAudio.h" )

set( ZP_AUDIO_IMPL_SOURCE
    "" )
set( ZP_AUDIO_IMPL_HEADER
    "" )

source_group( Headers FILES
    ${ZP_AUDIO_HEADER}
    ${ZP_AUDIO_IMPL_HEADER}
)
source_group( Source FILES
    ${ZP_AUDIO_SOURCE}
    ${ZP_AUDIO_IMPL_SOURCE}
)

# Physics Config
set( ZP_PHYSICS_SOURCE
    "src/Physics/zpPhysics.cpp" )
set( ZP_PHYSICS_HEADER
    "include/Physics/zpPhysics.h" )

set( ZP_PHYSICS_IMPL_SOURCE
    "" )
set( ZP_PHYSICS_IMPL_HEADER
    "" )


source_group( Headers FILES
    ${ZP_PHYSICS_HEADER}
    ${ZP_PHYSICS_IMPL_HEADER}
)
source_group( Source FILES
    ${ZP_PHYSICS_SOURCE}
    ${ZP_PHYSICS_IMPL_SOURCE}
)

# Rendering Config
set( ZP_RENDERING_SOURCE
    "src/Rendering/zpCamera.cpp"
    "src/Rendering/zpFont.cpp"
    "src/Rendering/zpMaterial.cpp"
    "src/Rendering/zpMesh.cpp"
    "src/Rendering/zpRenderCommandBuffer.cpp"
    "src/Rendering/zpRenderPipeline.cpp"
    "src/Rendering/zpRenderTarget.cpp"
    "src/Rendering/zpRenderingContext.cpp"
    "src/Rendering/zpRenderingEngine.cpp"
    "src/Rendering/zpShader.cpp"
    "src/Rendering/zpTexture.cpp" )
set( ZP_RENDERING_HEADER
    "include/Rendering/zpCamera.h"
    "include/Rendering/zpFont.h"
    "include/Rendering/zpMaterial.h"
    "include/Rendering/zpMesh.h"
    "include/Rendering/zpRenderCommandBuffer.h"
    "include/Rendering/zpRenderPipeline.h"
    "include/Rendering/zpRenderTarget.h"
    "include/Rendering/zpRendering.h"
    "include/Rendering/zpRenderingContext.h"
    "include/Rendering/zpRenderingEngine.h"
    "include/Rendering/zpShader.h"
    "include/Rendering/zpTexture.h" )

source_group( Headers FILES
    ${ZP_RENDERING_HEADER}
)
source_group( Source FILES
    ${ZP_RENDERING_SOURCE}
)

# RenderingOpenGL Config
set( ZP_RENDERING_OPENGL_SOURCE
    "src/RenderingOpenGL/zpRenderingOpenGL.cpp" )
set( ZP_RENDERING_OPENGL_HEADER
    "include/RenderingOpenGL/zpRenderingOpenGL.h" )

source_group( Headers FILES
    ${ZP_RENDERING_OPENGL_HEADER}
)
source_group( Source FILES
    ${ZP_RENDERING_OPENGL_SOURCE}
)

# GLEW
set( ZP_RENDERING_GLEW_SOURCE 
    "src/GL/glew.c" )
set( ZP_RENDERING_GLEW_HEADER 
    "include/GL/glew.h" )
include_directories( "include" )

source_group( Headers FILES
    ${ZP_RENDERING_GLEW_HEADER}
)
source_group( Source FILES
    ${ZP_RENDERING_GLEW_SOURCE}
)

# RenderingD3D Config
file( GLOB ZP_RENDERING_D3D_SOURCE "src/RenderingD3D/*.cpp" )
file( GLOB ZP_RENDERING_D3D_HEADER "include/RenderingD3D/*.h" )

source_group( Headers FILES
    ${ZP_RENDERING_D3D_HEADER}
)
source_group( Source FILES
    ${ZP_RENDERING_D3D_SOURCE}
)

# Rendering Impl OpenGL Config
if( ZP_USE_OPENGL_RENDERING )
    set( ZP_RENDERING_IMPL_SOURCE ${ZP_RENDERING_OPENGL_SOURCE} ${ZP_RENDERING_GLEW_SOURCE} )
    set( ZP_RENDERING_IMPL_HEADER ${ZP_RENDERING_OPENGL_HEADER} ${ZP_RENDERING_GLEW_HEADER} )
endif( ZP_USE_OPENGL_RENDERING )

# Rendering Impl D3D Config
if( ZP_USE_D3D_RENDERING )
    set( ZP_RENDERING_IMPL_SOURCE ${ZP_RENDERING_D3D_SOURCE} )
    set( ZP_RENDERING_IMPL_HEADER ${ZP_RENDERING_D3D_HEADER} )
    set( ZP_RENDERING_IMPL_INLINE ${ZP_RENDERING_D3D_INLINE} )
endif( ZP_USE_D3D_RENDERING )

# All Config
#set( ZP_ALL_HEADER
#    ${ZP_CORE_HEADER}
#    ${ZP_ENGINE_HEADER}
#    ${ZP_RENDERING_HEADER}
#    ${ZP_RENDERING_IMPL_HEADER}
#)
#set( ZP_ALL_INLINE
#    ${ZP_CORE_INLINE}
#    ${ZP_ENGINE_INLINE}
#    ${ZP_RENDERING_INLINE}
#    ${ZP_RENDERING_IMPL_INLINE}
#)
#set( ZP_ALL_SOURCE
#    ${ZP_CORE_SOURCE}
#    ${ZP_ENGINE_SOURCE}
#    ${ZP_RENDERING_SOURCE}
#    ${ZP_RENDERING_IMPL_SOURCE}
#)

# Static Lib build
if( ZP_BUILD_STATIC )
    #if( ZP_BUILD_SINGLE )
    #    add_library( ZeroPoint5_lib STATIC
    #        ${ZP_ALL_HEADER}
    #        ${ZP_ALL_SOURCE}
    #    )
    #    
    #    set_property( TARGET ZeroPoint5_lib PROPERTY FOLDER "Libraries" )
    #    set_target_properties( ZeroPoint5_lib PROPERTIES
    #        OUTPUT_NAME ${PROJECT_NAME}
    #        CLEAN_DIRECT_OUTPUT 1
    #        VERSION ${ZP_VERSION}
    #    )
    #endif( ZP_BUILD_SINGLE )

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

    # Engine
    add_library( ZeroPoint5_Engine_lib STATIC
        ${ZP_ENGINE_HEADER}
        ${ZP_ENGINE_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Engine_lib PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Engine_lib PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Engine
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )

    # Audio
    add_library( ZeroPoint5_Audio_lib STATIC
        ${ZP_AUDIO_HEADER}
        ${ZP_AUDIO_SOURCE}
        ${ZP_AUDIO_IMPL_HEADER}
        ${ZP_AUDIO_IMPL_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Audio_lib PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Audio_lib PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Audio
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )

    # Physics
    add_library( ZeroPoint5_Physics_lib STATIC
        ${ZP_PHYSICS_HEADER}
        ${ZP_PHYSICS_SOURCE}
        ${ZP_PHYSICS_IMPL_HEADER}
        ${ZP_PHYSICS_IMPL_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Physics_lib PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Physics_lib PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Physics
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )

    # Rendering
    add_library( ZeroPoint5_Rendering_lib STATIC
        ${ZP_RENDERING_HEADER}
        ${ZP_RENDERING_SOURCE}
        ${ZP_RENDERING_IMPL_HEADER}
        ${ZP_RENDERING_IMPL_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Rendering_lib PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Rendering_lib PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Rendering
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
        COMPILE_DEFINITIONS GLEW_STATIC
    )
endif( ZP_BUILD_STATIC )

# Shared DLL build
if( ZP_BUILD_SHARED )
    #if( ZP_BUILD_SINGLE )
    #    add_library( ZeroPoint5_dll SHARED
    #        ${ZP_ALL_HEADER}
    #        ${ZP_ALL_SOURCE}
    #    )
    #    
    #    set_property( TARGET ZeroPoint5_dll PROPERTY FOLDER "Libraries" )
    #    set_target_properties( ZeroPoint5_dll PROPERTIES
    #        OUTPUT_NAME ${PROJECT_NAME}
    #        CLEAN_DIRECT_OUTPUT 1
    #        VERSION ${ZP_VERSION}
    #    )
    #endif( ZP_BUILD_SINGLE )

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

    # Engine
    add_library( ZeroPoint5_Engine_dll SHARED
        ${ZP_ENGINE_HEADER}
        ${ZP_ENGINE_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Engine_dll PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Engine_dll PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Engine
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )

    # Audio
    add_library( ZeroPoint5_Audio_dll SHARED
        ${ZP_AUDIO_HEADER}
        ${ZP_AUDIO_SOURCE}
        ${ZP_AUDIO_IMPL_HEADER}
        ${ZP_AUDIO_IMPL_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Audio_dll PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Audio_dll PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Audio
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )

    # Physics
    add_library( ZeroPoint5_Physics_dll SHARED
        ${ZP_PHYSICS_HEADER}
        ${ZP_PHYSICS_SOURCE}
        ${ZP_PHYSICS_IMPL_HEADER}
        ${ZP_PHYSICS_IMPL_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Physics_dll PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Physics_dll PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Physics
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )

    # Rendering
    add_library( ZeroPoint5_Rendering_dll SHARED
        ${ZP_RENDERING_HEADER}
        ${ZP_RENDERING_SOURCE}
        ${ZP_RENDERING_IMPL_HEADER}
        ${ZP_RENDERING_IMPL_SOURCE}
    )

    set_property( TARGET ZeroPoint5_Rendering_dll PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_Rendering_dll PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}_Rendering
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
        COMPILE_DEFINITIONS GLEW_BUILD
    )
endif( ZP_BUILD_SHARED )

# Install
if( ZP_INSTALL )
    install( FILES ${ZP_CORE_HEADER} ${ZP_CORE_INLINE} DESTINATION include/Core )
    install( FILES ${ZP_ENGINE_HEADER} ${ZP_ENGINE_INLINE} DESTINATION include/Engine )

    if( ZP_BUILD_SINGLE )
        if( ZP_BUILD_STATIC )
            install( TARGETS ZeroPoint5_lib
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib
            )
        endif( ZP_BUILD_STATIC )

        if( ZP_BUILD_SHARED )
            install( TARGETS ZeroPoint5_dll
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib
            )
        endif( ZP_BUILD_SHARED )
    endif( ZP_BUILD_SINGLE )
endif( ZP_INSTALL )
