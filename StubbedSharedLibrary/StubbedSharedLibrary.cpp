#include "StubbedSharedLibrary.h"
#include "Library.h"

CStubbedSharedLibrary::CStubbedSharedLibrary( const std::string & string )
{
    setString( string );
}

CStubbedSharedLibrary::~CStubbedSharedLibrary()
{

}

void CStubbedSharedLibrary::setString( const std::string & string )
{
    fString = reverseString( string );
}

void create( const std::string & string, CStubbedSharedLibrary *& retVal )
{
    retVal = new CStubbedSharedLibrary( string );
}

void getString( CStubbedSharedLibrary * lib, std::string & retVal )
{
    retVal.clear();
    if( lib )
        retVal = lib->string();
}

void destroy( CStubbedSharedLibrary * lib )
{
    delete lib;
}
