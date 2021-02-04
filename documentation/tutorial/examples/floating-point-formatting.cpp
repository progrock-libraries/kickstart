#include <kickstart/all.hpp>
using namespace kickstart::all;
using ascii::at_right_in;

auto main() -> int
{
    double x = math::pi;
    for( int i = 1; i <= 5; ++i, x *= 1000 ) {
        out << at_right_in( 20, str( x ) )
            << at_right_in( 20, to_fixed( x, 4 ) )
            << at_right_in( 20, to_scientific( x, 4 ) )
            << endl;
    }
}
