#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    const fsx::Path path = fsx::exe_relative_path( "poem.txt" );
    auto reader = Text_reader( path );
    while( const auto line = reader.input_or_none() ) { out << line << "\n"; }
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
