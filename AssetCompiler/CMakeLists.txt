
set( ASSET_COMPILER_VERSION_MAJOR 0 )
set( ASSET_COMPILER_VERSION_MINOR 0 )
set( ASSET_COMPILER_VERSION_PATCH 1 )
set( ASSET_COMPILER_VERSION ${ASSET_COMPILER_VERSION_MAJOR}.${ASSET_COMPILER_VERSION_MINOR}.${ASSET_COMPILER_VERSION_PATCH} )

project( AssetCompiler VERSION ${ASSET_COMPILER_VERSION} LANGUAGES C CXX )

# AssetCompiler Config
set( ASSET_COMPILER_BIN ${PROJECT_NAME}_${PLATFORM_NAME}_${PLATFORM_ARCH} )

set( ZP_ASSET_COMPILER_SOURCE 
    "src/acApplication.cpp"
    "src/acCommandLineProcessor.cpp"
    "src/acMain.cpp"
    "src/acTextureCompiler.cpp"
)
set( ZP_ASSET_COMPILER_HEADER 
    "include/acApplication.h"
    "include/acAssetCompiler.h"
    "include/acCommandLineProcessor.h"
    "include/acMain.h"
    "include/acTextureCompiler.h"
)

source_group( Headers FILES
    ${ZP_ASSET_COMPILER_HEADER}
)
source_group( Source FILES
    ${ZP_ASSET_COMPILER_SOURCE}
)
include_directories( "${ZeroPoint5_SOURCE_DIR}/include" )
include_directories( "include" )
if( WIN32 )
include_directories( BEFORE SYSTEM
    "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
)
endif()

add_executable( AssetCompiler
    ${ZP_ASSET_COMPILER_HEADER}
    ${ZP_ASSET_COMPILER_SOURCE}
)

if( ZP_BUILD_STATIC )
target_link_Libraries( AssetCompiler
    ZeroPoint5_Core_lib
)
endif()

if( ZP_BUILD_SHARED )
target_link_Libraries( AssetCompiler
    ZeroPoint5_Core_dll
)
endif()

if( WIN32 )
set_target_properties( AssetCompiler PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS" )
endif()

set_target_properties( AssetCompiler PROPERTIES
    FOLDER "Executables"
    OUTPUT_NAME ${ASSET_COMPILER_BIN}
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${ASSET_COMPILER_VERSION}
)

install( TARGETS AssetCompiler RUNTIME DESTINATION bin )
