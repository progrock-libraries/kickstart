#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    const string name = input( "Hi, what’s your name? " );
    out << "Welcome to the Kickstart experience, " << name << "!" << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
