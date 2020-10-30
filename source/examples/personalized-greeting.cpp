#include <kickstart/kickstart.core.hpp>
using namespace kickstart;

void cpp_main()
{
    const string username = input( "Hi, what's your name? ");
    output( ""s << "Greetings, " << username << "! Happy to meet you!\n" );
}

auto main() -> int { return with_exceptions_displayed( cpp_main ); }
