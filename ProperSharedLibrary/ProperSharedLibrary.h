#ifndef __PROPER_SHARED_LIBRARY_H
#define __PROPER_SHARED_LIBRARY_H

#include <string>

/* this must be kept in sync with vxcClient.h which is exported to the world */
#ifndef PROPER_SHARED_LIBRARY_DECLSPEC
#    if (defined(_MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)) /* is windows */
#        ifdef PROPER_SHARED_LIBRARY_INTERNAL
#            define PROPER_SHARED_LIBRARY_DECLSPEC __declspec(dllexport)
#        else /* PROPER_SHARED_LIBRARY_INTERNAL */
#            define PROPER_SHARED_LIBRARY_DECLSPEC __declspec(dllimport)
#        endif /* PROPER_SHARED_LIBRARY_INTERNAL */
#    else  /* is windows */
#        define PROPER_SHARED_LIBRARY_DECLSPEC
#    endif /* is windows */
     /* QDECLSPEC is no longer needed but is defined for historical reasons */
#    define PROPER_SHARED_LIBRARY_QDECLSPEC PROPER_SHARED_LIBRARY_DECLSPEC
     /* Please use the PROPER_SHARED_LIBRARY_ however, to ease the transition, the VXC_ versions are defined as well */
#endif /* PROPER_SHARED_LIBRARY_DECLSPEC */

class PROPER_SHARED_LIBRARY_DECLSPEC CProperSharedLibrary
{
public:
    CProperSharedLibrary( const std::string & string );
    virtual ~CProperSharedLibrary();

    void setString( const std::string & string );
    std::string string() const { return fString; }
private:
    std::string fString;
};

PROPER_SHARED_LIBRARY_DECLSPEC void create( const std::string & string, CProperSharedLibrary *& retVal );
PROPER_SHARED_LIBRARY_DECLSPEC void getString( CProperSharedLibrary * lib, std::string & retVal );
PROPER_SHARED_LIBRARY_DECLSPEC void destroy( CProperSharedLibrary * lib );
#endif

