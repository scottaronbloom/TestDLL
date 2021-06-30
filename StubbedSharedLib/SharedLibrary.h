#ifndef __SHARED_LIBRARY_H
#define __SHARED_LIBRARY_H

#include <string>

/* this must be kept in sync with vxcClient.h which is exported to the world */
#ifndef SHARED_LIBRARY_DECLSPEC
#    if (defined(_MSC_VER) || defined(__MINGW32__) || defined(__CYGWIN__)) /* is windows */
#        ifdef SHARED_LIBRARY_INTERNAL
#            define SHARED_LIBRARY_DECLSPEC __declspec(dllexport)
#        else /* SHARED_LIBRARY_INTERNAL */
#            define SHARED_LIBRARY_DECLSPEC __declspec(dllimport)
#        endif /* SHARED_LIBRARY_INTERNAL */
#    else  /* is windows */
#        define SHARED_LIBRARY_DECLSPEC
#    endif /* is windows */
     /* QDECLSPEC is no longer needed but is defined for historical reasons */
#    define SHARED_LIBRARY_QDECLSPEC SHARED_LIBRARY_DECLSPEC
     /* Please use the SHARED_LIBRARY_ however, to ease the transition, the VXC_ versions are defined as well */
#endif /* SHARED_LIBRARY_DECLSPEC */

class SHARED_LIBRARY_DECLSPEC CSharedLibary
{
public:
    CSharedLibary( const std::string & string );
    virtual ~CSharedLibary();

    void setString( const std::string & string );
    std::string string() const { return fString; }
private:
    std::string fString;
};
#endif

