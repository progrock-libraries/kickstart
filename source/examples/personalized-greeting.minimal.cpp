// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/kickstart.core.hpp>
using namespace kickstart;

auto main() -> int
{
    const string username = input( "Hi, what’s your name? ");
    output( ""s << "Greetings, " << username << "! Happy to meet you!\n" );
}
