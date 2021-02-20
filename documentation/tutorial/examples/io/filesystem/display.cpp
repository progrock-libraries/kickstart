#include <kickstart/all.hpp>
using namespace kickstart::all;

#include <kickstart/system-specific/console-adapted-io-functions.hpp>
using kickstart::system_specific::output_to_console;

#include <fstream>
using std::ifstream;

const auto& cmd = process::the_commandline();

auto help_text()
    -> string
{ return "Usage: "s << ::cmd.verb() << " FILEPATH"; }
    
auto filename_from_commandline()
    -> const string&
{
    const auto& args = cmd.args();
    switch( args.size() ) {
        case 1: {
            return args.item( 0 );
        }
        default: {
            fail_app_with_message( help_text() );
        }
    }
    unreachable();          // To avoid a Visual C++ sillywarning.
}

void cppmain()
{
    const string& filename = filename_from_commandline();
    
    // Works for e.g. “π.txt” in Windows.
    const optional<C_file> of = fsx::open_c_file( fsx::fspath_from_u8( filename ), "r" );
    hopefully( of.has_value() )
        or fail_app_with_message( "Unable to open “"s << filename << "” for reading." );
    const C_file f = of.value();

    while( auto line = any_input_from( f ) ) {
        output_to_console( stdout, line.value() + "\n" );
    }
    hopefully( !!::feof( f ) )
        or fail_app_with_message( "Reading from “"s << filename << "” failed." );
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
