#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain( const string_view& cmd_verb, const Cmdline_args& cmd_args )
{
    out << "“" << cmd_verb << "”" << endl;
    for( const auto arg: cmd_args ) {
        out << "“" << arg << "”" << endl;
    }
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
