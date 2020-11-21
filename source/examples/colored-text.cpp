// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/core.hpp>
using namespace kickstart::all;

namespace escape_seq {
    const char      escape  = char( 27 );
    const string    lead_in = ""s << escape << '[';

    auto color( const int n ) -> string { return ""s << lead_in << n << "m"; }
}  // namespace escape_seq

auto main() -> int
{
    using escape_seq::color;
    const string red        = color( 91 );
    const string yellow     = color( 93 );
    const string blue       = color( 96 );
    const string restore    = color( 0 );
                   
    output( "Hello, colored world!\n" );
    output( ""s
        << "Examples: "
        << red << "red" << restore << ", "
        << yellow << "yellow" << restore << " and "
        << blue << "blue" << restore << " (or cyan, really) colored text.\n"
        );
}
