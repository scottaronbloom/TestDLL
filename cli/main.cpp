#include "Library.h"
#include "SharedLibrary.h"

#include <iostream>

int main( int argc, char ** argv )
{
    for( int ii = 0; ii < argc; ++ii )
    {
        CSharedLibary sharedLib( argv[ ii ] );
        std::cout << "OrigString: " << argv[ ii ] << " Modified String Via Shared Lib: '" << sharedLib.string() << "'" << std::endl;
        std::cout << "OrigString: " << argv[ ii ] << " Modified String Via Static Lib: '" << reverseString( argv[ ii ] ) << "'" << std::endl;

    }
}