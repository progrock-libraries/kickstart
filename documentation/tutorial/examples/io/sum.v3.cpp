#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    out << "This will calculate the sum of numbers like 2.17 and 3.14." << endl;
    out << "Enter one or more numbers per line. Just"
           " press return to calculate the sum." << endl;

    vector<double> numbers;
    out << endl;
    for( ;; ) {
        const string spec = input( "Numbers (or just return), please? " );
        if( spec == "" ) {
            break;
        }
        for( const double x: parts_to_vector_<double>( spec ) ) {
            numbers.push_back( x );
        }
    }

    out << endl;
    for( const double& x: numbers ) {
        out << (&x > &numbers.front()? " + " : "") << x;
    }
    out << " = " << math::sum_of( numbers ) << "." << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
