// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/assertion-headers/~assert-reasonable-compiler.hpp>

// Copyright (c) 2020 Alf P. Steinbach. MIT license, with license text:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <kickstart/main_library/console/utf8-io/io.hpp>
#include <kickstart/main_library/core/ns▸text_conversion/to-text.hpp>

namespace kickstart::utf8_output_operator::_definitions {
    using namespace kickstart::utf8_io;
    using namespace kickstart::text_conversion;

    struct Faux_cout_stream {};
    struct Faux_cerr_stream {};
    struct Faux_endl {};

    inline auto operator<<( Faux_cout_stream, const Faux_endl& )
        -> Faux_cout_stream
    {
        output( "\n" );
        return {};
    }

    template< class Value >
    inline auto operator<<( Faux_cout_stream, const Value& v )
        -> Faux_cout_stream
    {
        output( str( v ) );
        return {};
    }

    inline auto operator<<( Faux_cerr_stream, const Faux_endl& )
        -> Faux_cout_stream
    {
        output_error_message( "\n" );     // Already flushes every item.
        return {};
    }

    template< class Value >
    inline auto operator<<( Faux_cerr_stream, const Value& v )
        -> Faux_cout_stream
    {
        output_error_message( str( v ) );
        return {};
    }

    inline const auto   out     = Faux_cout_stream();
    inline const auto   err     = Faux_cerr_stream();
    inline const auto   endl    = Faux_endl();


    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::out,
        d::err,
        d::endl;
    }  // namespace exported names
}  // namespace kickstart::utf8_io::_definitions

namespace kickstart::utf8_output_operator   { using namespace _definitions::exported_names; }
