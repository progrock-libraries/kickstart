#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    const auto&         filename    = "poem.txt";
    const fsx::Path     path        = fsx::path_of_exe_relative( filename );
    
    auto reader = Text_reader( path );
    while( const optional<string> line = reader.input_or_eof() ) {
        out << line << "\n";
    }
    hopefully( reader.has_passed_eof() )
        or fail_app( "Reading from “"s << filename << "” failed." );
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
