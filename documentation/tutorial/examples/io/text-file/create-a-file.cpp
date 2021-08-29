#include <kickstart/all.hpp>
using namespace kickstart::all;

const auto& text =
    "Every international blueberry jam lover knows that\n"
    "every 日本国 кошка loves …\n"
    "\n"
    "▷ Norwegian “blåbærsyltetøy”! ◁ 😋\n";

void cppmain()
{
    const auto& cmd = process::the_commandline();
    hopefully( cmd.args().size() == 1 )
        or fail_app( ""s << "Usage: “" << cmd.verb() << " FILENAME”." );
    const string_view& path = cmd.args().at( 0 );
    Text_writer( path ).output( text );
    out << "Saved some text in “" << path << "”." << endl;
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
