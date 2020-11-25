#include <kickstart/with_utf8_streams.hpp>
using namespace kickstart::with_utf8_streams;

void cpp_main()
{
    out     << "This program computes the sum of two numbers A and B like "
            << 2.72 << " and " << 3.14 << "."
            << endl;

    out << endl;
    const double    a   = to_double( input( "Number A, please: " ) );
    const double    b   = to_double( input( "Number B, please: " ) );
    
    out << endl;
    out << a << " + " << b << " = " << a + b << "."  << endl;
}

auto main() -> int { return with_exceptions_displayed( cpp_main ); }
