#include "acMain.h"

acCommandLineProcessor::acCommandLineProcessor()
{

}

acCommandLineProcessor::~acCommandLineProcessor()
{

}

zp_int acCommandLineProcessor::process( const zp_char* cmdLine )
{
    zp_printfln( cmdLine );
    return 0;
}

void acCommandLineProcessor::addFlag( const zp_char* shortName, zp_bool required )
{
    acCommandLineEntry flag = {};
    flag.isRequired = required;
    flag.minParameterCount = 0;
    flag.maxParameterCount = 0;

    m_config.set( zpString( shortName ), flag );
}

void acCommandLineProcessor::addParameter( const zp_char* shortName, zp_bool required, zp_size_t minParameterCount, zp_size_t maxParameterCount )
{
    acCommandLineEntry flag = {};
    flag.isRequired = required;
    flag.minParameterCount = minParameterCount;
    flag.maxParameterCount = maxParameterCount;

    m_config.set( zpString( shortName ), flag );
}

zp_bool acCommandLineProcessor::hasFlag( const zp_char* shortName ) const
{
    return m_commandLineParts.containsKey( zpString( shortName ) );
}

zp_bool acCommandLineProcessor::getParameters( const zp_char* shortName, zpVector<zpString>& outParameters ) const
{
    zp_bool found = false;

    acCommandLinePart* part;
    if( m_commandLineParts.get( zpString( shortName ), part ) )
    {
        if( part->length > 0 )
        {
            found = true;
            
            outParameters.reserve( part->length );

            for( zp_size_t i = 0; i < part->length; ++i )
            {
                outParameters.pushBack( m_commandLineTokens[ i + part->offset ] );
            }
        }
    }

    return found;
}
