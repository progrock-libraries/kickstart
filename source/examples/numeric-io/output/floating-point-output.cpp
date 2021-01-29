#include <kickstart/all.hpp>
#include <kickstart/assertion-headers/¤-assert-posix-number-names-support.hpp>
using namespace kickstart::all;

#include <math.h>

auto main() -> int
{
    const double pi = M_PI;
    out << "Default presentation:" << endl;
    for( int oom = 1; oom <= 10000; oom *= 10 ) {
        out << "  " << pi*oom << endl;
    }

    out << endl;
    out << "Fixed point notation with 9 decimals:" << endl;
    for( int oom = 1; oom <= 10000; oom *= 10 ) {
        out << "  " << to_fixed( pi*oom, 9 ) << endl;
    }

    out << endl;
    out << "Scientific notation with 9 decimals:" << endl;
    for( int oom = 1; oom <= 10000; oom *= 10 ) {
        out << "  " << to_scientific( pi*oom, 9 ) << endl;
    }
}
