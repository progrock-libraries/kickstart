#include <kickstart/all.hpp>
using namespace kickstart::all;

using M = kickstart::matrices::Matrix_<double>;

auto main() -> int
{
    const Matrix_<double> m =
    {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    out << "Matrix size: " << m.width() << "×" << m.height() << "." << endl;
    out << "Values: " << endl;
    out << endl;
    for( int y = 0; y < m.height(); ++y ) {
        for( int x = 0; x < m.width(); ++x ) {
            out << ascii::at_right_in( 4, str( m({ x, y }) ) );
        }
        out << endl;
    }
}
