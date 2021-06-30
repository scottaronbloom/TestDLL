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
        std::string dir = "C:\\Visual Studio\\Projects\\github\\scottaronbloom\\TestDLL\\build\\" + fDllName + "\\Debug";
        AddDllDirectory( std::wstring( dir.begin(), dir.end() ).c_str() );
        fModule = LoadLibrary( fDllNameWExt.c_str() );
        if( !fModule )
        {
            std::cerr << "could not find " << fDllNameWExt << " in path" << std::endl;
            std::cerr << getLastError( "LoadLibrary(" + fDllNameWExt + ")" ) << std::endl;
            return;
        }
        char path[ MAX_PATH ]{ 0 };
        if( GetModuleFileName( fModule, path, sizeof( path ) ) == 0 )
        {
            std::cerr << getLastError( "GetModuleFileName" ) << std::endl;
            return;
            // Return or however you want to handle an error.
        }
        fPath = path;

        fCreateAddress = (CREATE_PROC)resolve( "create", "?create@@YAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEAPEAV" + fClassName + "@@@Z" );
        fDestroyAddress = (DESTROY_PROC)resolve( "destroy", "?destroy@@YAXPEAV" + fClassName + "@@@Z" );
        fGetStringAddress = (GETSTRING_PROC)resolve( "getString", "?getString@@YAXPEAV" + fClassName + "@@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z" );
    }

    virtual ~CSharedLibraryWrapperLinux()
    {
        destroy();
        UnloadLib();
    }

    virtual void UnloadLib() override
    {
        FreeLibrary( fModule );
    }

    FUNC_PTR resolve( const std::string & funcName, const std::string & mangledName ) override
    {
        auto address = mangledName.empty() ? GetProcAddress( fModule, funcName.c_str() ) : GetProcAddress( fModule, mangledName.c_str() );
        if( !address )
        {
            auto dispName = funcName;
            if( !mangledName.empty() )
            {
                char undName[ 4096 ];
                if( UnDecorateSymbolName( mangledName.c_str(), undName, sizeof( undName ), UNDNAME_COMPLETE ) )
                {
                    dispName = undName;
                }
                else
                {
                    std::cerr << "could not undecorate the function" << std::endl;
                    std::cerr << "    " << getLastError( "UnDecorateSymbolName(" + mangledName + ")" ) << std::endl;
                    return address;
                }
            }
            std::cerr << "could not find create proc address for function '" << dispName << "'" << std::endl;
            std::cerr << "    " << getLastError( "GetProcAddress(" + dispName + ")" ) << std::endl;
        }
        return address;
    }
};

#endif
