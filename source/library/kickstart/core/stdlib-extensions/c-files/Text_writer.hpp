// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

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

#include <kickstart/core/language/Tag_.hpp>
#include <kickstart/core/language/Truth.hpp>
#include <kickstart/core/stdlib-extensions/c-files/Abstract_c_file.hpp>
#include <kickstart/core/stdlib-extensions/filesystem/Path.hpp>
#include <kickstart/core/stdlib-extensions/strings.hpp>     // starts_with
#include <kickstart/core/text-conversion/to-text/string-output-operator.hpp>
#include <kickstart/core/text-encoding/utf8/bom.hpp>

namespace kickstart::tag {
    using kickstart::language::Tag_;
    using Append_to = Tag_<struct Struct_append_to>;
}  // namespace kickstart::tag

namespace kickstart::c_files::_definitions {
    using namespace kickstart::text_conversion;     // ""s, string operator<<
    using namespace kickstart::strings;             // starts_with
    using kickstart::language::Truth;

    class Text_writer:
        public Abstract_c_file
    {
        Truth                       m_some_output_done;
        utf8::Bom_policy::Enum      m_bom_policy;

        Text_writer(
            const fsx::Path&                path,
            const utf8::Bom_policy::Enum    bom_policy,
            const C_str                     mode
            ):
            Abstract_c_file( open_c_file_or_x( path, mode ) ),
            m_some_output_done( false ),
            m_bom_policy( bom_policy )
        {}

    public:
        Text_writer(
            const fsx::Path&                path,
            const utf8::Bom_policy::Enum    bom_policy  = utf8::system_default_bom_policy
            ):
           Text_writer( path, bom_policy, "w" )
        {}

        Text_writer(
            tag::Append_to,
            const fsx::Path&                path,
            const utf8::Bom_policy::Enum    bom_policy  = utf8::system_default_bom_policy
            ):
            Text_writer( path, bom_policy, "a" )
        {}

        void output( const string_view& s )
        {
            if( s.length() > 0 ) {
                if( not m_some_output_done ) {
                    if( m_bom_policy == utf8::Bom_policy::with_bom and
                        not starts_with( utf8::bom_sv, s )
                        ) {
                        clib_output_to( c_file(), utf8::bom_sv );
                        m_some_output_done = true;
                    }
                }
                clib_output_to( c_file(), s );
                m_some_output_done = true;
            }
        }

        void flush()
        {
            ::fflush( c_file() );
        }
    };

    namespace d = _definitions;
    namespace exports{ using
        d::Text_writer;

        namespace tag = kickstart::tag;
    }  // exports
}  // namespace kickstart::c_files::_definitions

namespace kickstart::c_files    { using namespace _definitions::exports; }
