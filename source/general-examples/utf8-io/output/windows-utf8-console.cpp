#ifndef _WIN32
#   error "This file is for Windows systems only."
#endif

#include <stdlib.h>     // system
#include <string>       // std::(to_string, string)

namespace {
    using namespace std::string_literals;
    using   std::string, std::to_string;

    auto cmd( const string& s )
        -> int
    { return ::system( s.c_str() ); }

    void set_codepage( const int id )
    {
        cmd( ""s + "chcp " + to_string( id ) + " >nul" );
    }

    auto get_codepage()
        -> int
    { return cmd( "for /f \"tokens=4\" %t in ('chcp') do @exit %t" ); }

    struct Console_codepage_fix
    {
        int original_codepage;

        ~Console_codepage_fix() { set_codepage( original_codepage ); }
        
        Console_codepage_fix():
            original_codepage( get_codepage() )
        {
            set_codepage( 65001 );
        }
    };
    
    const Console_codepage_fix the_fix = {};
}  // namespace <anon>
