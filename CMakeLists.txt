cmake_minimum_required( VERSION 3.6 )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
project( ZeroPoint LANGUAGES C CXX )

set( PLATFORM_NAME Win )
set( PLATFORM_ARCH x64 )

add_subdirectory( ZeroPoint5 )
add_subdirectory( Blackjack )
add_subdirectory( AssetCompiler )
