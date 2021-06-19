#pragma once    // Source encoding: UTF-8 (π is a lowercase Greek "pi").
// #include <c/setjmp.hpp>
//
// Macro (and function) that saves (and jumps) to an execution context.
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#ifndef REALLY_WANT_SETJMP
#   error `setjmp` doesn't work well with C++. But you can define REALLY_WANT_SETJMP.
#   include <because.setjmp_does_not_work_well_with_cplusplus>
#endif

#include <csetjmp>
#include <setjmp.h>
