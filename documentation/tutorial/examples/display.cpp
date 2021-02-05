#include <kickstart/all.hpp>
using namespace kickstart::all;

#include <fstream>
using std::ifstream;

const auto& cmd = process::the_commandline();

auto help_text()
    -> string
{ return "Usage: "s << ::cmd.verb() << " [-file] FILEPATH"; }
    
auto filename_from_commandline()
    -> const string&
{
    const auto& args = ::cmd.args();
    switch( args.size() ) {
        case 1: {
            const string& arg = args.item( 0 );
            if( arg == "-?" or arg == "--help" or arg == "/?" ) {
                fail_app_with_message( help_text() );
            }
            if( starts_with( '-', arg ) ) {
                fail_app_with_message( ""s
                    << "Invalid option “" << arg << "”.\n"
                    << help_text()
                    );
            }
            return arg;
        }
        case 2: {
            if( args.item( 0 ) == "-file" ) {
                return args.item( 1 );
            }
            fail_app_with_message( help_text() );
        }
        [[fallthrough]];    // To avoid a g++ sillywarning.
        default: {
            fail_app_with_message( help_text() );
        }
    }
    unreachable();          // To avoid a Visual C++ sillywarning.
}

void cppmain()
{
    const string& filename = filename_from_commandline();
    auto f = ifstream( fsx::u8_stdpath( filename ) );   // Works for e.g. “π.txt” in Windows.
    hopefully( not f.fail() )
        or fail_app_with_message( "Unable to open “"s << filename << "” for reading." );
        
    constexpr string_view byte_order_mark = "\uFEFF";
    string line;
    bool is_first_line = true;
    while( getline( f, line ) ) {
        if( is_first_line and starts_with( byte_order_mark, line ) ) {
            line = line.substr( byte_order_mark.size() );
        }
        out << line << endl;
        is_first_line = false;
    }
    hopefully( f.eof() )
        or fail_app_with_message( "Reading from “"s << filename << "” failed." );
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
