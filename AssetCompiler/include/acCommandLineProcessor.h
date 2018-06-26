#pragma once
#ifndef AC_COMMAND_LINE_PROCESSOR_H
#define AC_COMMAND_LINE_PROCESSOR_H

class acCommandLineProcessor
{
public:
    acCommandLineProcessor();
    ~acCommandLineProcessor();

    zp_int process( const zp_char* cmdLine );

    void addFlag( const zp_char* shortName, zp_bool required );
    void addParameter( const zp_char* shortName, zp_bool required, zp_size_t minParameterCount, zp_size_t maxParameterCount );

    zp_bool hasFlag( const zp_char* shortName ) const;
    zp_bool getParameters( const zp_char* shortName, zpVector<zpString>& outParameters ) const;

private:
    
    struct acCommandLineEntry
    {
        zp_bool isRequired;
        zp_size_t minParameterCount;
        zp_size_t maxParameterCount;
    };

    struct acCommandLinePart
    {
        zp_size_t offset;
        zp_size_t length;
    };

    zpVector<zpString> m_commandLineTokens;
    zpMap<zpString, acCommandLineEntry> m_config;
    zpMap<zpString, acCommandLinePart> m_commandLineParts;
};

#endif // !AC_COMMAND_LINE_PROCESSOR_H
