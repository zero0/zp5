#include "acMain.h"

acApplication::acApplication()
    : m_exitCode( 0 )
{
}

acApplication::~acApplication()
{
}

zp_int acApplication::processCommandLine( const zp_char* cmdLine )
{
    m_cmd.addFlag( "", false );

    zp_int r = m_cmd.process( cmdLine );
    return r;
}

zp_int acApplication::run()
{
    zp_int r = m_exitCode;
    return r;
}

void acApplication::exit( zp_int exitCode )
{
    m_exitCode = exitCode;
}

void acApplication::setup()
{
}

void acApplication::teardown()
{
}
