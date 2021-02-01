#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    const auto& cmd = process::the_commandline();

    out << cmd.verb() << endl;
    for( const string& arg: cmd.args() ) {
        out << arg << endl;
    }
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
