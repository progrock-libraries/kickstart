// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/all.hpp>
using namespace kickstart_utf8;

namespace ansi_escape_seq {
    const char      escape  = char( 27 );
    const string    lead_in = ""s << escape << '[';

    auto color( const int n ) -> string { return ""s << lead_in << n << "m"; }

    inline namespace colors {
        const string red        = color( 91 );
        const string yellow     = color( 93 );
        const string blue       = color( 96 );
        const string restore    = color( 0 );
    }  // namespace colors
}  // namespace ansi_escape_seq

auto main() -> int
{
    using namespace ansi_escape_seq::colors;
    out << "Hello, colored world!" << endl;
    out << "Examples: "
        << red << "red" << restore << ", "
        << yellow << "yellow" << restore << " and "
        << blue << "blue" << restore << " (or cyan, really) colored text."
        << endl;
}
