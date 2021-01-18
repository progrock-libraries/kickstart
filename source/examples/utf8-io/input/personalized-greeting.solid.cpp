// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#include <kickstart/all.hpp>
KS_ASSERT_UTF8_LITERALS();
using namespace kickstart::all;

void cpp_main()
{
    const string        answer      = ascii::trimmed( input( "Hi, what’s your name? ") );
    const string_view   username    = (answer.empty()? "“Nomen Nescio”"sv : answer);

    output( ""s << "Greetings, " << username << "! Happy to meet you!\n" );
}

auto main() -> int { return with_exceptions_displayed( cpp_main ); }
