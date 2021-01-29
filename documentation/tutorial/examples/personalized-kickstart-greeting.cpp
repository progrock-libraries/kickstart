#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    const string name = input( "Hi, what’s your name? " );
    out << "Welcome to the Kickstart experience, " << name << "!" << endl;
}
