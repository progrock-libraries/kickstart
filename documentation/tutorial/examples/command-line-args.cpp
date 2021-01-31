#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain( const string& verb, const Array_span_<const string>& args )
{
    out << verb << endl;
    for( const string& arg: args ) {
        out << arg << endl;
    }
}

auto main() -> int { return with_command_line_parts( cppmain ); }
