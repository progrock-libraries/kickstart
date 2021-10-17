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

#include <kickstart/main_library/core/ns▸collection_utility/collection-pointers.hpp>
#include <kickstart/main_library/core/failure-handling.hpp>
#include <kickstart/main_library/core/stdlib-extensions/files/ns▸c_files/C-file-types.hpp>
#include <kickstart/main_library/core/ns▸language/types/Truth.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace kickstart::c_files::_definitions {
    using namespace collection_utility;
    using namespace failure_handling;
    using namespace language;           // Truth etc.

    using   std::optional,
            std::string,
            std::string_view;

    inline auto clib_input_or_none_from( const C_file f )
        -> optional<string>
    {
        string  line;
        int     code;

        hopefully( not ferror( f ) )
            or KS_FAIL( "`ferror(f)` reports true for the file" );
        while( (code = fgetc( f )) != EOF and code != '\n' ) {
            line += char( code );
        }
        // NO check of error here in order to allow a partial read to succeed.
        const Truth eof = (line.empty() and code == EOF);
        if( eof ) { return {}; }
        return line;
    }

    inline void clib_output_to( const C_file f, const string_view& s )
    {
        const size_t n_bytes_written = ::fwrite( begin_ptr_of( s ), 1, s.size(), f );
        hopefully( n_bytes_written == s.size() )
            or KS_FAIL( "::fwrite failed" );
    }

    namespace d = _definitions;
    namespace exports{ using
        d::C_file,
        d::clib_input_or_none_from,
        d::clib_output_to;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files        { using namespace _definitions::exports; }
