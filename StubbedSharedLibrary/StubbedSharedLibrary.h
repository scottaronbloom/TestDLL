#ifndef __STUBBED_SHARED_LIBRARY_H
#define __STUBBED_SHARED_LIBRARY_H

#include <string>

/* this must be kept in sync with vxcClient.h which is exported to the world */
#ifndef STUBBED_SHARED_LIBRARY_DECLSPEC
#    if (defined(_MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)) /* is windows */
#        ifdef STUBBED_SHARED_LIBRARY_INTERNAL
#            define STUBBED_SHARED_LIBRARY_DECLSPEC __declspec(dllexport)
#        else /* STUBBED_SHARED_LIBRARY_INTERNAL */
#            define STUBBED_SHARED_LIBRARY_DECLSPEC __declspec(dllimport)
#        endif /* STUBBED_SHARED_LIBRARY_INTERNAL */
#    else  /* is windows */
#        define STUBBED_SHARED_LIBRARY_DECLSPEC
#    endif /* is windows */
     /* QDECLSPEC is no longer needed but is defined for historical reasons */
#    define STUBBED_SHARED_LIBRARY_QDECLSPEC STUBBED_SHARED_LIBRARY_DECLSPEC
     /* Please use the STUBBED_SHARED_LIBRARY_ however, to ease the transition, the VXC_ versions are defined as well */
#endif /* STUBBED_SHARED_LIBRARY_DECLSPEC */

class STUBBED_SHARED_LIBRARY_DECLSPEC CStubbedSharedLibrary
{
public:
    CStubbedSharedLibrary( const std::string & string );
    virtual ~CStubbedSharedLibrary();

    void setString( const std::string & string );
    std::string string() const { return fString; }
private:
    std::string fString;
};

#ifdef STUBBED_SHARED_LIBRARY_INTERNAL
extern "C" STUBBED_SHARED_LIBRARY_DECLSPEC void create( const std::string & string, CStubbedSharedLibrary *& retVal );
extern "C" STUBBED_SHARED_LIBRARY_DECLSPEC void getString( CStubbedSharedLibrary * lib, std::string & retVal );
extern "C" STUBBED_SHARED_LIBRARY_DECLSPEC void destroy( CStubbedSharedLibrary * lib );
#endif

#endif

