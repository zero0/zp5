#pragma once
#ifndef AC_COMMAND_LINE_PROCESSOR_H
#define AC_COMMAND_LINE_PROCESSOR_H

class acCommandLineProcessor
{
public:
    acCommandLineProcessor();
    ~acCommandLineProcessor();

    void addFlag( const zp_char* shortName, zp_bool required );
    void addParameter( const zp_char* shortName, zp_bool required, zp_size_t minParameterCount, zp_size_t maxParameterCount );

private:

};

#endif // !AC_COMMAND_LINE_PROCESSOR_H
