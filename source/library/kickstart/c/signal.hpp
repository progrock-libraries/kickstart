#pragma once    // Source encoding: UTF-8 (π is a lowercase Greek "pi").
// #include <c/signal.hpp>
//
// Functions and macro constants for signal management.
// Copyright © 2017 Alf P. Steinbach, distributed under Boost license 1.0.

#ifndef REALLY_WANT_SIGNAL
#   error `signal` doesn't work well with C++. You can define REALLY_WANT_SIGNAL.
#   include <because.signal_does_not_work_well_with_cplusplus>
#endif

#include <csignal>
#include <signal.h>
