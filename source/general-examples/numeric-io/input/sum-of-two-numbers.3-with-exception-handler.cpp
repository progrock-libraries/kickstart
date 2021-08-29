﻿#include <kickstart/all.hpp>
using namespace kickstart::all;

void cpp_main()
{
    out << "This program computes the sum of two numbers A and B like "
        << 2.72 << " and " << 3.14 << "."
        << endl;

    out << endl;
    const auto  a   = to_<double>( input( "Number A, please: " ) );
    const auto  b   = to_<double>( input( "Number B, please: " ) );
    
    out << endl;
    out << a << " + " << b << " = " << a + b << "."  << endl;
}

auto main() -> int { return with_exceptions_displayed( cpp_main ); }
