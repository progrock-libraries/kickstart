#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    out << "This will calculate the sum of numbers like 2.17 and 3.14." << endl;
    out << "Enter one number per line. Just press return to calculate the sum." << endl;

    vector<double> numbers;
    out << endl;
    for( ;; ) {
        const string spec = input( "A number (or just return), please? " );
        if( spec == "" ) {
            break;
        }
        numbers.push_back( to_<double>( spec ) );
    }
    
    out << endl;
    for( const double& x: numbers ) {
        out << (&x > &numbers.front()? " + " : "") << x;
    }
    out << " = " << math::sum_of( numbers ) << "." << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
