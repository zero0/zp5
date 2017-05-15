#pragma once
#ifndef AC_APPLICATION_H
#define AC_APPLICATION_H

#include "Core\zpCore.h"

class acApplication
{
public:
    acApplication();
    ~acApplication();

    zp_int processCommandLine( const zp_char* cmdLine );
    zp_int run();

private:

};

#endif
