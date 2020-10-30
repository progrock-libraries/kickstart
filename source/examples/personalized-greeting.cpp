// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/kickstart.core.hpp>
using namespace kickstart;

void cpp_main()
{
    const string answer = trimmed( input( "Hi, what's your name? ") );
    const string_view username = (answer.empty()? "“Nomen Nescio”"sv : answer);
    output( ""s << "Greetings, " << username << "! Happy to meet you!\n" );
}

auto main() -> int { return with_exceptions_displayed( cpp_main ); }
