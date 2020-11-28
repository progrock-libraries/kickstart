// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/with_utf8_streams.hpp>
using namespace kickstart_utf8;

auto main() -> int
{
    using namespace ansi_escape_seq::colors;
    out << "Hello, colored world!" << endl;
    out << "Examples: "
        << bright_red << "red" << restore << ", "
        << bright_yellow << "yellow" << restore << " and "
        << bright_blue << "blue" << restore << " colored text."
        << endl;
}
