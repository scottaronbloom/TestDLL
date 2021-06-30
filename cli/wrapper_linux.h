#ifndef __WRAPPER_LINUX_H
#define __WRAPPER_LINUX_H

#include "wrapper.h"
#include <string>

#include <dbghelp.h>

template< typename T >
class CSharedLibraryWrapperLinux : public CSharedLibraryWrapper< T >
{
public:
    CSharedLibraryWrapperLinux( const std::string & dllName ) :
        CSharedLibraryWrapper< T >( dllName )
    {
        auto dir = "~/sb/github/TestDll/build/" + fDllName + "//Debug";

        int dlFlags = 0;
        dlFlags |= RTLD_NOW;
        dlFlags |= RTLD_GLOBAL;
      
        fPath = dir + "//" + fDllNameWExt;
        fModule = dlopen( fPath.c_str(), dlFlags);
        if( !fModule )
        {
            std::cerr << "could not find " << fDllNameWExt << " in path" << std::endl;
            std::cerr << getLastError( "LoadLibrary(" + fDllNameWExt + ")" ) << std::endl;
            return;
        }

        fCreateAddress = (CREATE_PROC)resolve( "create", QString() );
        fDestroyAddress = (DESTROY_PROC)resolve( "destroy", QString() );
        fGetStringAddress = (GETSTRING_PROC)resolve( "getString", QString() );
    }

    virtual ~CSharedLibraryWrapperLinux()
    {
        destroy();
        UnloadLib();
    }

    virtual void UnloadLib() override
    {
        dlclose( fModule );
    }

    FUNC_PTR resolve( const std::string & funcName, const std::string & mangledName ) override
    {
        auto address = mangledName.empty() ? dlsym( fModule, funcName.c_str() ) : dlsym( fModule, mangledName.c_str() );
        if( !address )
        {
            auto dispName = funcName;
            std::cerr << "could not find create proc address for function '" << dispName << "'" << std::endl;
            std::cerr << "    " << getLastError( "GetProcAddress(" + dispName + ")" ) << std::endl;
        }
        return address;
    }
};

#endif
