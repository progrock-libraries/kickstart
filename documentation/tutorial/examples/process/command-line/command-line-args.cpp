#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    const auto& cmd = process::the_commandline();

    out << cmd.verb() << endl;
    for( const string& arg: cmd.args() ) {
        out << arg << endl;
    }
}
