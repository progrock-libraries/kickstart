#include <kickstart/all.hpp>
using namespace kickstart::all;

#include <random>

auto main() -> int
{
    Matrix_<double> m( 10, 10 );
    for( int y = 0; y < m.height(); ++y ) {
        for( int x = 0; x < m.width(); ++x ) {
            m( x, y ) = (x + y) % 10;
        }
    }

    out << "Matrix size: " << m.width() << "×" << m.height() << "." << endl;
    out << "Values: " << endl;
    out << endl;
    for( int y = 0; y < m.height(); ++y ) {
        for( int x = 0; x < m.width(); ++x ) {
            out << ascii::at_right_in( 4, str( m( x, y ) ) );
        }
        out << endl;
    }
}
