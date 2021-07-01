#ifndef __WRAPPER_LINUX_H
#define __WRAPPER_LINUX_H

#include "wrapper.h"
#include <string>
#include <dlfcn.h>
#include <iostream>

template< typename T >
class CSharedLibraryWrapperLinux : public CSharedLibraryWrapper< T >
{
public:
    CSharedLibraryWrapperLinux( const std::string & dllName ) :
        CSharedLibraryWrapper< T >( dllName )
    {
        this->fDllName = "lib" + dllName;
        this->fDllNameWExt = this->fDllName + ".so";

        int dlFlags = 0;
        dlFlags |= RTLD_NOW;
        dlFlags |= RTLD_GLOBAL;
      
        auto dir = "/home/sbloom/sb/github/scottaronbloom/TestDLL/build/" + dllName;
        this->fPath = dir + "/" + this->fDllNameWExt;
        this->fModule = dlopen( this->fPath.c_str(), dlFlags);
        if( !this->fModule )
        {
            std::cerr << "could not find " << this->fDllNameWExt << " in path" << std::endl;
            std::cerr << getLastError( "LoadLibrary(" + this->fDllNameWExt + ")" ) << std::endl;
            return;
        }

        //using CREATE_PROC=CSharedLibraryWrapper< T >::CREATE_PROC;
        using CREATE_PROC=typename CSharedLibraryWrapper< T >::CREATE_PROC;
        using DESTROY_PROC=typename CSharedLibraryWrapper< T >::DESTROY_PROC;
        using GETSTRING_PROC=typename CSharedLibraryWrapper< T >::GETSTRING_PROC;

        this->fCreateAddress = (CREATE_PROC)resolve( "create", std::string() );
        this->fDestroyAddress = (DESTROY_PROC)resolve( "destroy", std::string() );
        this->fGetStringAddress = (GETSTRING_PROC)resolve( "getString", std::string() );
    }

    virtual ~CSharedLibraryWrapperLinux()
    {
        this->destroy();
        this->UnloadLib();
    }

    virtual void UnloadLib() override
    {
        if ( this->fModule )
            dlclose( this->fModule );
    }

    FUNC_PTR resolve( const std::string & funcName, const std::string & mangledName ) override
    {
        auto address = mangledName.empty() ? dlsym( this->fModule, funcName.c_str() ) : dlsym( this->fModule, mangledName.c_str() );
        if( !address && !mangledName.empty() )
            address = dlsym( this->fModule, funcName.c_str() );
        if ( !address )
        {
            auto dispName = funcName;
            std::cerr << "could not find create proc address for function '" << dispName << "'" << std::endl;
            std::cerr << "    " << getLastError( "GetProcAddress(" + dispName + ")" ) << std::endl;
        }
        return address;
    }
};

#endif
