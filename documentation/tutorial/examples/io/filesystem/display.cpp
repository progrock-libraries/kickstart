#include <kickstart/all.hpp>
using namespace kickstart::all;

auto& read_line = clib_input_or_eof_from;   // Like a functional `std::getline`.

void cppmain()
{
    const auto open_for_reading = []( const fsx::Path& path )
        -> C_file
    {
        if( const optional<C_file> of = fsx::open_c_file( path, "r" ) ) {
            return of.value();
        }
        fail_app( "Unable to open “"s << path.to_string() << "” for reading." );
        unreachable();
    };

    const auto&     filename    = "π.txt";
    const auto      path        = fsx::path_of_exe_directory() / filename;
    const C_file    f           = open_for_reading( path );

    while( const optional<string> line = read_line( f ) ) {
        out << line.value() << "\n";
    }
    hopefully( !!::feof( f ) )
        or fail_app( "Reading from “"s << filename << "” failed." );
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
