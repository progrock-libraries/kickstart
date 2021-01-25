#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    out << process::commandline() << endl;
}
