#include "Library.h"
#include <algorithm>

std::string reverseString( const std::string & inStr )
{
    std::string retVal = inStr;
    std::reverse( retVal.begin(), retVal.end() );
    return retVal;
}