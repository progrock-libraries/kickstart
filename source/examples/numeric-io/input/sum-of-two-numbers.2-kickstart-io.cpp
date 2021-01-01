#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main()
    -> int
{
    out << "This program computes the sum of two numbers A and B like "
        << 2.72 << " and " << 3.14 << "."
        << endl;

    out << endl;
    const double    a   = to_<double>( input( "Number A, please: " ) );
    const double    b   = to_<double>( input( "Number B, please: " ) );
    
    out << endl;
    out << a << " + " << b << " = " << a + b << "."  << endl;
}
