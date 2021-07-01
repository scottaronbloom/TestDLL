#ifndef __WRAPPER_H
#define __WRAPPER_H

#include <string>
std::string getCWD();
std::string getLastError( const std::string & funcName );

#ifdef _WIN32
#include <Windows.h>
#define LIB_PTR HMODULE
#define FUNC_PTR FARPROC
#else
#define LIB_PTR void *
#define FUNC_PTR void *
#endif

template< typename T >
class CSharedLibraryWrapper
{
public:
    typedef void ( *CREATE_PROC )( const std::string & string, T *& retVal );
    typedef void ( *GETSTRING_PROC )( T *, const std::string & );
    typedef void ( *DESTROY_PROC )( T * );

    CSharedLibraryWrapper( const std::string & dllName ) :
        fDllName( dllName ),
        fDllNameWExt( dllName + ".dll" ),
        fClassName( "C" + dllName )
    {
    }

    virtual ~CSharedLibraryWrapper()
    {
    }

    virtual FUNC_PTR resolve( const std::string & funcName, const std::string & mangledName ) = 0;
    virtual void UnloadLib() = 0;
    std::string path() const { return fPath; }

    virtual T * create( const std::string & string ) final
    {
        if( !fCreateAddress )
            return nullptr;
        fCreateAddress( string, fSharedLibrary );
        return fSharedLibrary;
    }

    virtual std::string string() final
    {
        if( !fSharedLibrary || !fGetStringAddress )
            return std::string();
        std::string retVal;
        fGetStringAddress( fSharedLibrary, retVal );
        return retVal;
    }

    virtual void destroy() final
    {
        if( !fSharedLibrary || !fDestroyAddress)
            return;
        fDestroyAddress( fSharedLibrary );
    }
    
protected:
    LIB_PTR fModule{ 0 };
    std::string fPath;
    CREATE_PROC fCreateAddress{ nullptr };
    DESTROY_PROC fDestroyAddress{ nullptr };
    GETSTRING_PROC fGetStringAddress{ nullptr };

    std::string fDllName;
    std::string fDllNameWExt;
    std::string fClassName;

    T * fSharedLibrary{ nullptr };
};

#endif

