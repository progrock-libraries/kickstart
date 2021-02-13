#include <kickstart/all.hpp>
using namespace kickstart::all;

#include <kickstart/system-specific/Console.hpp>
auto& console = kickstart::system_specific::the_console();

void cppmain()
{
    const string name = console.input( "Hi, what’s your name? " );
    const int age = to_<int>( console.input( "And please, your age (in years)? " ) );

    console.output( ""s
        << "Welcome to the Kickstart experience, "
        << age << " year" << (age == 1? "": "s")
        << " old "<< name << "!\n"
        );
}

auto main() -> int { return with_exceptions_displayed( cppmain ); }
