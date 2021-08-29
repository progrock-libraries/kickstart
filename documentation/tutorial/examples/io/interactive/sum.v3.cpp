#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    out << "This will calculate the sum of numbers like 2.17 and 3.14." << endl;
    out << "Enter one or more numbers per line. Just"
           " press return to calculate the sum." << endl;
    out << endl;

    auto numbers    = vector<double>();
    auto reader     = Text_parts_reader( "Numbers (or just return), please? " );
    while( const optional<string_view> spec = reader.input_part() ) {
        numbers.push_back( to_<double>( spec.value() ) );
    }

    out << endl;
    for( const double& x: numbers ) {
        out << (&x > &numbers.front()? " + " : "") << x;
    }
    out << " = " << math::sum_of( numbers ) << "." << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
