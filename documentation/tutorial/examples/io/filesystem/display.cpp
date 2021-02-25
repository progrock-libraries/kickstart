#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    const string_view filename = process::the_commandline().args().at( 0 );
    auto reader = Text_reader( filename );

    while( const optional<string> line = reader.input_or_eof() ) {
        out << line << "\n";
    }

    hopefully( reader.has_passed_eof() )
        or fail_app( "Reading from “"s << filename << "” failed." );
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
