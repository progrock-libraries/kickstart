#include <kickstart/all.hpp>
using namespace kickstart::all;

auto open_for_reading( const fsx::Path& path )
    -> C_file
{
    if( const optional<C_file> of = open_c_file( path, "r" ) ) {
        return of.value();
    }
    fail_app( "Unable to open “"s << path.to_string() << "” for reading." );
    unreachable();
}

void cppmain()
{
    const auto&     filename    = "π.txt";
    const auto      path        = fsx::path_of_exe_directory() / filename;
    const C_file    f           = open_for_reading( path );

    while( const optional<string> line = any_input_from( f ) ) {
        out << line.value() << "\n";
    }
    hopefully( !!::feof( f ) )
        or fail_app( "Reading from “"s << filename << "” failed." );
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
