﻿#pragma once    // Source encoding: UTF-8 (π is a lowercase Greek "pi").
// #include <c/tgmath.hpp>
//
// In C11 `tgmath.h` defines type-generic macros so that e.g. `fabs(x)` can be
// invoked with either floating point or complex argument. In C++ this is
// achieved via ordinary function overloading. I.e. <tgmath.h> is not meaningful
// in C++, but the C++ standard library provides it for compatibility.
//
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#include <ctgmath>          // Non-C++-compatible macro definitions + includes.
// #include <tgmath.h>      // Not available with Visual C++ 2017.
