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

#include <utility>

namespace kickstart::x_utility::_definitions {
    enum Template_paramameter_requirement_result_type {};

    // As a function one gets an up front error message with Visual C++.
    template< const bool v >
    constexpr inline auto template_parameter_requirement_()
        -> Template_paramameter_requirement_result_type
    {
        static_assert( v, "A template parameter requirement was not fulfilled" );
        return {};
    }

    namespace d = _definitions;
    namespace exported_names { using
        d::Template_paramameter_requirement_result_type,
        d::template_parameter_requirement_;
    }  // namespace exported names
}  // namespace kickstart::x_utility::_definitions 

namespace kickstart::x_utility    { using namespace _definitions::exported_names; }
