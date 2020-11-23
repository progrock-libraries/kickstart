#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main()
    -> int
{
    cout    << "This program computes the sum of two numbers A and B like "
            << 2.72 << " and " << 3.14 << "."
            << endl;

    cout << endl;
    const double    a   = to_double( input( "Number A, please: " ) );
    const double    b   = to_double( input( "Number B, please: " ) );
    
    cout << endl;
    cout << a << " + " << b << " = " << a + b << "."  << endl;
}
