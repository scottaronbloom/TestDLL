#include "Library.h"
#include "Wrapper_win.h"

#include "StubbedSharedLibrary.h"
#include "ProperSharedLibrary.h"

class CStubbedSharedLibrary;
class CProperSharedLibrary;

//#include <sstream>
#include <iostream>

//#include <libloaderapi.h>
//#include <errhandlingapi.h>
//#include <direct.h>
//#include <Windows.h>

int main( int argc, char ** argv )
{
#ifdef _WIN32
    SetDefaultDllDirectories( LOAD_LIBRARY_SEARCH_DEFAULT_DIRS );
    if( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) )
    {
        std::cerr << "could not initialize symbols" << std::endl;
        std::cerr << "    " << getLastError( "SymInitialize" ) << std::endl;
        return -1;
    }
#endif

    std::cout << "Running from: " << getCWD() << "\n";

    for( int ii = 0; ii < argc; ++ii )
    {
        auto stubbedLibWrapper = std::make_shared< CSharedLibraryWrapperWin< CStubbedSharedLibrary > >( "StubbedSharedLibrary" );
        auto properLibWrapper = std::make_shared< CSharedLibraryWrapperWin< CProperSharedLibrary > >( "ProperSharedLibrary" );

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