// Based on the example at <url: https://github.com/jarro2783/cxxopts#example>.
#ifdef __GNUC__
#   pragma GCC diagnostic ignored "-Wmissing-field-initializers"    // Sillywarning.
#endif

#include <kickstart/all.hpp>
using namespace kickstart::all;

#include "wrapped-cxxoptions.hpp"       // my::Options
using my::Options;

void cppmain()
{
    const auto& cmd = process::the_commandline();
    const auto options = Options( cmd.verb(), "A cxxopts command line options example.",
    {
        { "b,bar",      "Param bar",        Options::kind<string>() },
        { "d,debug",    "Enable debugging", Options::no_option_default( true ) },
        { "f,foo",      "Param foo",        Options::no_option_default( 10 ) },
        { "h,help",     "Print usage" }
    } );

    const auto parse_result = options.parse( cmd.c_strings() );
    if( cmd.args().size() == 0 or parse_result.count( "help" ) > 0 ) {
        out << options.help_text() << endl;
    } else {
        const auto& r = parse_result;

        const string    bar     = (r.count( "bar" ) > 0? r["bar"].as<string>() : "");
        const bool      debug   = r["debug"].as<bool>();
        const int       foo     = r["foo"].as<int>();

        out << "bar = “" << bar << "”"
            << ", debug = " << Options::spec_for( debug )
            << ", foo = " << foo
            << "." << endl;
        const vector<string>& rest = r.unmatched();
        if( rest.empty() ) {
            out << "Everything in the command line was matched as options." << endl;
        } else {
            out << "Unmatched command line parts:" << endl;
            for( const string& s: rest ) {
                out << "• “" << s << "”." << endl;
            }
        }
    }
}

auto main( int n_cmd_parts, char** cmd_parts )
    -> int
{ return with_exceptions_displayed( cppmain, n_cmd_parts, cmd_parts ); }
