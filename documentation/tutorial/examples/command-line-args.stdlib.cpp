#include <iostream>
#include <string>
#include <vector>
using namespace std;

void cppmain( const string& cmd_verb, const vector<string>& cmd_args )
{
    cout << cmd_verb << endl;
    for( const string& arg: cmd_args ) {
        cout << arg << endl;
    }
}

auto main( const int n_cmd_parts, char** const cmd_parts )
    -> int
{ cppmain( cmd_parts[0], vector<string>( cmd_parts + 1, cmd_parts + n_cmd_parts ) ); }
