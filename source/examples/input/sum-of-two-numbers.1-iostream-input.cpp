#include <iostream>
using   std::cin, std::cout, std::endl;

auto main()
    -> int
{
    cout    << "This program computes the sum of two numbers A and B like "
            << 2.72 << " and " << 3.14 << "."
            << endl;

    double a;
    cout << "Number A, please: ";  cin >> a;
    double b;
    cout << "Number B, please: ";  cin >> b;
    
    cout << endl;
    cout << a << " + " << b << " = " << a + b << "."  << endl;
}
