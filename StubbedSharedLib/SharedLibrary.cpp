#include "SharedLibrary.h"
#include "Library.h"

CSharedLibary::CSharedLibary( const std::string & string )
{
    setString( string );
}

CSharedLibary::~CSharedLibary()
{

}

void CSharedLibary::setString( const std::string & string )
{
    fString = reverseString( string );
}

