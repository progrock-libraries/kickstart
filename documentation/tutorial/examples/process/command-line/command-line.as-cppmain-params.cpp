#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain( const string& cmd_verb, const Array_span_<const string>& cmd_args )
{
    out << cmd_verb << endl;
    for( const string& arg: cmd_args ) {
        out << arg << endl;
    }
}

auto main() -> int { with_command_line_parts( cppmain ); }
