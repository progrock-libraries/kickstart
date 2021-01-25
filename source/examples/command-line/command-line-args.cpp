﻿#include <kickstart/all.hpp>
using namespace kickstart::all;

auto main() -> int
{
    const auto& commandline = process::commandline();
    out << "“" << commandline.verb() << "”" << endl;
    for( const auto arg: commandline.args() ) {
        out << "“" << arg << "”" << endl;
    }
}
