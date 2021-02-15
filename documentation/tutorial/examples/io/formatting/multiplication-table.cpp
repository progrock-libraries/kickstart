#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    const int n = 12;
    for( int row = 1; row <= n; ++row ) {
        for( int col = 1; col <= n; ++col ) {
            out << ascii::at_right_in( 4, str( row*col ) );
        }
        out << endl;
    }
}
