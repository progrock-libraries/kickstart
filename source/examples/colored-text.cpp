// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/with_utf8_streams.hpp>
using namespace kickstart_utf8;

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
