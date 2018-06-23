#pragma once
#ifndef AC_APPLICATION_H
#define AC_APPLICATION_H

class acApplication
{
public:
    acApplication();
    ~acApplication();

    zp_int processCommandLine( const zp_char* cmdLine );
    zp_int run();

    void exit( zp_int exitCode );

private:
    void setup();
    void teardown();

    zp_int m_exitCode;
};

#endif
