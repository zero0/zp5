
set( BLACKJACK_VERSION_MAJOR 0 )
set( BLACKJACK_VERSION_MINOR 0 )
set( BLACKJACK_VERSION_PATCH 1 )
set( BLACKJACK_VERSION ${BLACKJACK_VERSION_MAJOR}.${BLACKJACK_VERSION_MINOR}.${BLACKJACK_VERSION_PATCH} )

project( Blackjack VERSION ${BLACKJACK_VERSION} LANGUAGES C CXX )

# file( GLOB BLACKJACK_SOURCE "src/*.cpp" )
# file( GLOB BLACKJACK_HEADER "src/*.h" )
# file( GLOB BLACKJACK_INLINE "src/*.inl" )
set( BLACKJACK_SOURCE
    "src/bMain.cpp"
)
set( BLACKJACK_HEADER )
set( BLACKJACK_INLINE )

set( BLACKJACK_BIN ${PROJECT_NAME}_${PLATFORM_NAME}_${PLATFORM_ARCH} )
set( BLACKJACK_ASSETS "Assets/" )

source_group( Headers FILES
    ${BLACKJACK_HEADER}
    ${BLACKJACK_INLINE}
)
source_group( Source FILES
    ${BLACKJACK_SOURCE}
)
include_directories( "${ZeroPoint5_SOURCE_DIR}/include" )
if( WIN32 )
include_directories( BEFORE SYSTEM
    "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
)
endif()

add_executable( Blackjack
    ${BLACKJACK_HEADER}
    ${BLACKJACK_SOURCE}
)

if( ZP_BUILD_STATIC )
target_link_Libraries( Blackjack debug
    ZeroPoint5_Core_lib
    ZeroPoint5_Engine_lib
    ZeroPoint5_Rendering_lib
)
target_link_Libraries( Blackjack optimized
    ZeroPoint5_Core_lib
    ZeroPoint5_Engine_lib
    ZeroPoint5_Rendering_lib
)
endif()

if( ZP_BUILD_SHARED )
target_link_Libraries( Blackjack debug
    ZeroPoint5_Core_dll
    ZeroPoint5_Engine_dll
    ZeroPoint5_Rendering_dll
)
target_link_Libraries( Blackjack optimized
    ZeroPoint5_Core_dll
    ZeroPoint5_Engine_dll
    ZeroPoint5_Rendering_dll
)
endif()

if( WIN32 )
set_target_properties( Blackjack PROPERTIES 
    LINK_FLAGS "/SUBSYSTEM:WINDOWS" 
    VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
)
endif()

set_target_properties( Blackjack PROPERTIES
    FOLDER "Executables"
    OUTPUT_NAME ${BLACKJACK_BIN}
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${BLACKJACK_VERSION}
)

install( TARGETS Blackjack RUNTIME DESTINATION bin )
install( DIRECTORY Assets DESTINATION bin )
