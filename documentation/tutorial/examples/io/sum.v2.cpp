#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    out << "This will calculate the sum of numbers like 2.17 and 3.14." << endl;
    out << "Enter the numbers on one line, separated by spaces." << endl;

    out << endl;
    const string    spec    = input( "Numbers, please? " );
    const auto      numbers = parts_to_vector_<double>( spec );

    out << endl;
    for( const double& x: numbers ) {
        out << (&x > &numbers.front()? " + " : "") << x;
    }

    out << " = " << math::sum_of( numbers ) << "." << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
