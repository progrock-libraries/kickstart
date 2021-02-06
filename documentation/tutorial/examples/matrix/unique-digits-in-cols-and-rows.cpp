#include <kickstart/all.hpp>
using namespace kickstart::all;

#include <random>
using   std::random_device, std::mt19937, std::uniform_int_distribution;

auto main() -> int
{
    const int size = 10;
    Matrix_<double> m( size );

    // Every row and every column systematically filled with unique values.
    for( int y = 0; y < size; ++y ) { for( int x = 0; x < size; ++x ) {
        m( x, y ) = (x + y) % size;
    } }

    { // Random reordering of rows and columns.
        auto entropy        = random_device();
        auto random_bits    = mt19937( entropy() );
        auto index_from     = uniform_int_distribution( 0, size - 1 );
        for( int i = 0; i < size; ++i ) {
            swap_rows( i, index_from( random_bits ), m );
            swap_columns( i, index_from( random_bits ), m );
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
