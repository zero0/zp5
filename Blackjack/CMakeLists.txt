
set( BLACKJACK_VERSION_MAJOR 0 )
set( BLACKJACK_VERSION_MINOR 0 )
set( BLACKJACK_VERSION_PATCH 1 )
set( BLACKJACK_VERSION ${BLACKJACK_VERSION_MAJOR}.${BLACKJACK_VERSION_MINOR}.${BLACKJACK_VERSION_PATCH} )


project( Blackjack VERSION ${BLACKJACK_VERSION} LANGUAGES C CXX )

file( GLOB BLACKJACK_SOURCE "src/*.cpp" )
file( GLOB BLACKJACK_HEADER "src/*.h" )
file( GLOB BLACKJACK_INLINE "src/*.inl" )

set( BLACKJACK_BIN ${PROJECT_NAME}_Win64 )

source_group( Blackjack\\Headers FILES
    ${BLACKJACK_HEADER}
)
source_group( Blackjack\\Sourece FILES
    ${BLACKJACK_SOURCE}
    ${BLACKJACK_INLINE}
)
include_directories( ${ZeroPoint5_SOURCE_DIR} )

add_executable( Blackjack
    ${BLACKJACK_HEADER}
    ${BLACKJACK_SOURCE}
)

target_link_Libraries( Blackjack
    ZeroPoint5_lib
)

IF( WIN32 )
set_target_properties( Blackjack PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS" )
ENDIF()

set_target_properties( Blackjack PROPERTIES
    FOLDER "Executables"
    OUTPUT_NAME ${BLACKJACK_BIN}
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${BLACKJACK_VERSION}
)

install( TARGETS Blackjack RUNTIME DESTINATION bin )
