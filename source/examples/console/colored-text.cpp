// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/all.hpp>
using namespace kickstart::all;

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
