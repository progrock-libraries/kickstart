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

// Iostreams intentionally not included by default, it's large.
// It can be included via `<kickstart/with_iostreams.hpp>`.

#include <kickstart/main_library/core/ns▸collection_support.hpp>
#include <kickstart/main_library/core/stdlib-extensions/files.hpp>
#include <kickstart/main_library/core/stdlib-extensions/ns▸basic_type_properties.hpp>
#include <kickstart/main_library/core/stdlib-extensions/ns▸calc.hpp>
#include <kickstart/main_library/core/stdlib-extensions/failure-handling.hpp>
#include <kickstart/main_library/core/stdlib-extensions//ns▸strings.hpp>
#include <kickstart/main_library/core/stdlib-extensions/ns▸type_traits.hpp>
