
set( ZP_VERSION_MAJOR 0 )
set( ZP_VERSION_MINOR 0 )
set( ZP_VERSION_PATCH 1 )
set( ZP_VERSION ${ZP_VERSION_MAJOR}.${ZP_VERSION_MINOR}.${ZP_VERSION_PATCH} )

project( ZeroPoint5 VERSION ${ZP_VERSION} LANGUAGES C CXX )

option( ZP_INSTALL "Install ZeroPoint5" OFF )
option( ZP_BUILD_SHARED "Build ZeroPoint5 as shared library" OFF )
option( ZP_BUILD_STATIC "Build ZeroPoint5 as static library" ON )

option( ZP_USE_ASSERTIONS "Build ZeroPoint5 with assersion" ON )
option( ZP_USE_PRINT "Build ZeroPoint5 with prints" ON )
option( ZP_USE_SAFE_FUNCTIONS "Build ZeroPoint5 with safe functions" ON )
option( ZP_USE_SIMD "Build ZeroPoint5 with SIMD" ON )

configure_file(
    "${PROJECT_SOURCE_DIR}/Core/zpConfig.h.in"
    "${PROJECT_BINARY_DIR}/Core/zpConfig.h"
)

file( GLOB ZP_CORE_SOURCE "Core/*.cpp" )
file( GLOB ZP_CORE_HEADER "Core/*.h" )

source_group( Core FILES
    ${ZP_CORE_SOURCE} ${ZP_CORE_HEADER}
)

if( ZP_BUILD_STATIC )
    add_library( ZeroPoint5_lib STATIC
        ${ZP_CORE_HEADER}
        ${ZP_CORE_SOURCE}
    )

    set_property( TARGET ZeroPoint5_lib PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_lib PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )
endif( ZP_BUILD_STATIC )

if( ZP_BUILD_SHARED )
    add_library( ZeroPoint5_dll SHARED
        ${ZP_CORE_HEADER}
        ${ZP_CORE_SOURCE}
    )

    set_property( TARGET ZeroPoint5_dll PROPERTY FOLDER "Libraries" )
    set_target_properties( ZeroPoint5_dll PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        CLEAN_DIRECT_OUTPUT 1
        VERSION ${ZP_VERSION}
    )
endif( ZP_BUILD_SHARED )

if( ZP_INSTALL )
    install( FILES ${ZP_CORE_HEADER} DESTINATION include/Core )

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
