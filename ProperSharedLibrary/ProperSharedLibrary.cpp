#include "ProperSharedLibrary.h"
#include "Library.h"

CProperSharedLibrary::CProperSharedLibrary( const std::string & string )
{
    setString( string );
}

CProperSharedLibrary::~CProperSharedLibrary()
{

}

void CProperSharedLibrary::setString( const std::string & string )
{
    fString = reverseString( string );
}

void create( const std::string & string, CProperSharedLibrary *& retVal )
{
    retVal = new CProperSharedLibrary( string );
}

void getString( CProperSharedLibrary * lib, std::string & retVal )
{
    retVal.clear();
    if( lib )
        retVal = lib->string();
}

void destroy( CProperSharedLibrary * lib )
{
    delete lib;
}
