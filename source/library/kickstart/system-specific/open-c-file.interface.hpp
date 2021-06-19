﻿// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
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

#include <kickstart/core/language/type-aliases.hpp>             // C_str
#include <kickstart/core/stdlib-extensions/strings.hpp>         // C_str_ref
#include <kickstart/core/stdlib-extensions/c-files/clib-file-types.hpp>

#include <filesystem> 
#include <optional>

namespace kickstart::system_specific::_definitions {
    using kickstart::c_files::C_file;
    using kickstart::language::C_str;
    using kickstart::strings::C_str_ref;

    namespace fs = std::filesystem;

    using   std::optional;

    inline auto raw_u8open_c_file( const C_str utf8_path, const C_str mode )
        -> C_file;

    inline auto raw_open_c_file( const fs::path& path, const C_str mode )
        -> C_file;

    inline auto u8open_c_file( const C_str_ref utf8_path, const C_str_ref mode )
        -> optional<C_file>
    {
        const C_file f = raw_u8open_c_file( utf8_path.s(), mode.s() );
        if( not f ) { return {}; }
        return f;
    }

    inline auto open_c_file( const fs::path& path, const C_str_ref mode )
        -> optional<C_file>
    {
        const C_file f = raw_open_c_file( path, mode.s() );
        if( not f ) { return {}; }
        return f;
    }

    namespace d = _definitions;
    namespace exports { using
        d::u8open_c_file,
        d::open_c_file;
    }  // namespace exports
}  // namespace kickstart::system_specific::_definitions

namespace kickstart::system_specific { using namespace _definitions::exports; }
