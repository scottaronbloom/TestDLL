#include "Library.h"
#include "StubbedSharedLibrary.h"
#include "ProperSharedLibrary.h"

class CStubbedSharedLibrary;
class CProperSharedLibrary;

#include <sstream>
#include <iostream>

#include <libloaderapi.h>
#include <errhandlingapi.h>
#include <direct.h>
#include <Windows.h>
#include <dbghelp.h>

std::string getCWD()
{
    std::string retVal;
    char * buffer = nullptr;
    if( ( buffer = _getcwd( NULL, 0 ) ) == NULL )
        perror( "_getcwd error" );
    else
    {
        retVal = buffer;
        free( buffer );
    }
    return retVal;
}

std::string getLastError( const std::string & funcName )
{
    auto errCode = GetLastError();

    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errCode,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPTSTR)&lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process


    std::ostringstream oss;
    oss << funcName << " failed with error " << errCode << ": " << lpMsgBuf;

    LocalFree( lpMsgBuf );
    return oss.str();
}

template< typename T >
class CSharedLibraryWrapper
{
public:
    typedef void ( *CREATE_PROC )( const std::string & string, T *& retVal );
    typedef void ( *GETSTRING_PROC )( T *, const std::string & );
    typedef void ( *DESTROY_PROC )( T * );

    CSharedLibraryWrapper( const std::string & dllName )
    {
        std::string dir = "C:\\Visual Studio\\Projects\\github\\scottaronbloom\\TestDLL\\build\\" + dllName + "\\Debug";
        AddDllDirectory( std::wstring( dir.begin(), dir.end() ).c_str() );
        auto dllNameWExt = dllName + ".dll";
        auto className = "C" + dllName;
        fModule = LoadLibrary( dllNameWExt.c_str() );
        if( !fModule )
        {
            std::cerr << "could not find " << dllNameWExt << " in path" << std::endl;
            std::cerr << getLastError( "LoadLibrary(" + dllNameWExt + ")" ) << std::endl;
            return;
        }
        char path[ MAX_PATH ];
        if( GetModuleFileName( fModule, path, sizeof( path ) ) == 0 )
        {
            std::cerr << getLastError( "GetModuleFileName" ) << std::endl;
            return;
            // Return or however you want to handle an error.
        }
        fPath = path;

        fCreateAddress = (CREATE_PROC)resolve( "create", "?create@@YAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEAPEAV" + className + "@@@Z" );
        fDestroyAddress = (DESTROY_PROC)resolve( "destroy", "?destroy@@YAXPEAV" + className + "@@@Z" );
        fGetStringAddress = (GETSTRING_PROC)resolve( "getString", "?getString@@YAXPEAV" + className + "@@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z" );
    }

    virtual ~CSharedLibraryWrapper()
    {
        if( fSharedLibrary )
            destroy();
        FreeLibrary( fModule );
    }

    FARPROC resolve( const std::string & funcName, const std::string & mangledName )
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
    std::string path() const { return fPath; }

    T * create( const std::string & string )
    {
        if( !fCreateAddress )
            return nullptr;
        fCreateAddress( string, fSharedLibrary );
        return fSharedLibrary;
    }

    std::string string()
    {
        if( !fSharedLibrary || !fGetStringAddress )
            return std::string();
        std::string retVal;
        fGetStringAddress( fSharedLibrary, retVal );
        return retVal;
    }

    void destroy()
    {
        if( !fSharedLibrary || !fDestroyAddress)
            return;
        fDestroyAddress( fSharedLibrary );
    }
private:
    HMODULE fModule{ 0 };
    std::string fPath;
    CREATE_PROC fCreateAddress{ nullptr };
    DESTROY_PROC fDestroyAddress{ nullptr };
    GETSTRING_PROC fGetStringAddress{ nullptr };

    T * fSharedLibrary{ nullptr };
};


int main( int argc, char ** argv )
{
    SetDefaultDllDirectories( LOAD_LIBRARY_SEARCH_DEFAULT_DIRS );
    if( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) )
    {
        std::cerr << "could not initialize symbols" << std::endl;
        std::cerr << "    " << getLastError( "SymInitialize" ) << std::endl;
        return -1;
    }

    std::cout << "Running from: " << getCWD() << "\n";

    for( int ii = 0; ii < argc; ++ii )
    {
        auto stubbedLibWrapper = std::make_shared< CSharedLibraryWrapper< CStubbedSharedLibrary > >( "StubbedSharedLibrary" );
        auto properLibWrapper = std::make_shared< CSharedLibraryWrapper< CProperSharedLibrary > >( "ProperSharedLibrary" );

        if( ii == 0 )
        {
            std::cout
                << "Found Stubbed Shared Library at '" << stubbedLibWrapper->path() << std::endl
                << "Found Proper Shared Library at '" << properLibWrapper->path() << std::endl
                << "===================================================\n"
                ;
        }

        std::cout << "Original String: '" << argv[ ii ] << "\n";

        auto dynLoadedStubbed = stubbedLibWrapper->create( argv[ ii ] );
        std::cout << "    Dynamically Loaded Stubbed Library Results: '" << stubbedLibWrapper->string() << "'\n";

        auto dynLoadedProper = properLibWrapper->create( argv[ ii ] );
        std::cout << "    Dynamically Loaded Proper Library Results: '" << properLibWrapper->string() << "'\n";

        auto stubbedLibStatic = new CStubbedSharedLibrary( argv[ ii ] );
        std::cout << "    Statically bound Stubbed Library Results: '" << stubbedLibStatic->string() << "'\n";

        auto properLibStatic = new CProperSharedLibrary( argv[ ii ] );
        std::cout << "    Statically bound Proper Library Results: '" << properLibStatic->string() << "'\n";
    }

}