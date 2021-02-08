// The example at <url: https://github.com/jarro2783/cxxopts#example>, reworked.
#include <cxxopts.hpp>
#include <kickstart/all.hpp>
using namespace kickstart::all;

const auto& cmd =  process::the_commandline();
 
auto make_options_spec()
    -> cxxopts::Options
{
    cxxopts::Options result( "cxxopts-test", "A cxxopts command line options example." );
    result.add_options()
        ( "b, bar",     "Param bar",        cxxopts::value<string>() )
        ( "d, debug",   "Enable debugging", cxxopts::value<bool>()->default_value( "false" ) )
        ( "f, foo",     "Param foo",        cxxopts::value<int>()->default_value( "10" ) )
        ( "h, help",    "Print usage" );
    return result;
}

auto parse_command_line( const cxxopts::Options& spec )
    -> cxxopts::ParseResult
{
    auto&       mutable_spec    = const_cast<cxxopts::Options&>( spec );
    const auto  c_strings       = cmd.c_strings();     // An Array_span_

    return mutable_spec.parse( int_size( c_strings ), begin_ptr_of( c_strings ) );
}

void cppmain()
{
    const cxxopts::Options      spec    = make_options_spec();
    const cxxopts::ParseResult  result  = parse_command_line( spec );

    if( cmd.args().size() == 0 or result.count( "help" ) > 0 ) {
        out << spec.help() << endl;
    } else {
        const auto& r = result;

        const bool      debug   = r["debug"].as<bool>();
        const string    bar     = (r.count( "bar" ) > 0? r["bar"].as<string>() : "");
        const int       foo     = r["foo"].as<int>();

        out << "bar = “" << bar << "”"
            << ", debug = " << (debug? "true" : "false")
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
