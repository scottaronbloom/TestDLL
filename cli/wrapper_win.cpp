#include "wrapper.h"

#include <sstream>

std::string getLastError( const std::string & funcName )
{
    auto errCode = GetLastError();

    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errCode,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPTSTR)&lpMsgBuf,
        0, NULL );

    std::ostringstream oss;
    oss << funcName << " failed with error " << errCode << ": " << lpMsgBuf;

    LocalFree( lpMsgBuf );
    return oss.str();
}
