// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once

// Can't just include "reasonable-compiler" assert because it uses this file.
#include <kickstart/assertion-headers/~assert-is-c++17-or-later.hpp>
#include <kickstart/assertion-headers/~assert-has-boolean-operator-keywords.hpp>

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

// Can't use the `Truth` class here because it would be a circular include.
// #include <kickstart/root/core/language/Truth.hpp>

#include <kickstart/root/core/language/Default.hpp>              // Default
#include <kickstart/root/system-specific/os-detection.hpp>       // os_is_windows

#include <string_view>

namespace kickstart::utf8::_definitions {
    using   kickstart::language::Default;
    using   std::string_view;

    constexpr auto& bom     = "\uFEFF";
    constexpr auto  bom_sv  = string_view( bom );

    struct Bom_policy{ enum Enum{ no_bom, with_bom }; };    // For files.

    const auto system_default_bom_policy    = (0?Default()
        : system_specific::os_is_windows?   Bom_policy::with_bom
        :                                   Bom_policy::no_bom
        );

    namespace d = _definitions;
    namespace exports{ using
        d::bom,
        d::bom_sv,
        d::Bom_policy,
        d::system_default_bom_policy;
    }  // namespace exports
}  // namespace kickstart::utf8::_definitions

namespace kickstart::utf8   { using namespace _definitions::exports; }
