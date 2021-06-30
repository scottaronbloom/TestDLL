#include "wrapper.h"

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

#include <iostream>

std::string getCWD()
{
    std::string retVal;
    char * buffer = nullptr;
    if( ( buffer = _getcwd( NULL, 0 ) ) == NULL )
        std::cerr << "getcwd error" << std::endl;
    else
    {
        retVal = buffer;
        free( buffer );
    }
    return retVal;
}
