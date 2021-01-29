#include <iostream>
#include <string>
using namespace std;

auto fail() -> bool { throw "Gah!"; }

auto main() -> int
{
    string name;
    cout << "Hi, what’s your name? ";
    getline( cin, name ) or fail();
    cout << "Welcome to the C++ standard library experience, " << name << "!" << endl;
}
