#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    auto p1 = fsx::Path( "" );
    auto p2 = fsx::Path( string_view() );
    auto p3 = fsx::Path( string() );
    auto p4 = fsx::Path( std::filesystem::path() );

    (void) p1; (void) p2; (void) p3; (void) p4;
}
