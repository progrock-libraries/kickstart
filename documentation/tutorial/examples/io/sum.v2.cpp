#include <kickstart/all.hpp>
using namespace kickstart::all;

auto sum_of( const vector<double>& numbers )
    -> double
{
    double result = 0;
    for( const double x : numbers ) { result += x; }
    return result;
}

void cppmain()
{
    out << "Will calculate the sum of two or more numbers like 2.17 and 3.14." << endl;

    const string    number_specs    = input( "Enter two or more numbers, please: " );
    const auto      numbers         = parts_to_vector_<double>( number_specs );

    for( const double& x: numbers ) {
        out << (&x != &numbers.front()? " + " : "") << x;
    }
    out << " = " << sum_of( numbers ) << "." << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
