#include <kickstart/all.hpp>
using namespace kickstart::all;

void cppmain()
{
    const string name = input( "Hi, what’s your name? " );
    const int age = to_<int>( input( "And please, your age (in years)? " ) );

    out << "Welcome to the Kickstart experience, "
        << age << " year" << (age == 1? "": "s")
        << " old "<< name << "!" << endl;
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
