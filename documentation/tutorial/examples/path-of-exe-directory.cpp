#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    out << fsx::path_of_exe_directory().to_string() << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
