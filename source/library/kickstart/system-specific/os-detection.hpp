// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/core/language/assertion-headers/~assert-reasonable-compiler.hpp>

#undef      KS_OS_IS_WINDOWS
#undef          KS_OS_IS_WIN64
#undef          KS_OS_IS_WIN32
#undef      KS_OS_IS_UNIX
#undef          KS_OS_IS_LINUX
#undef          KS_OS_IS_IOS
#undef              KS_OS_IS_IOS_IN_XCODE_SIMULATOR
#undef              KS_OS_IS_IOS_ON_DEVICE
#undef          KS_OS_IS_OSX

#if defined( _WIN64 )
#   define KS_OS_IS_WIN64           1
#   define KS_OS_IS_WINDOWS         1
#elif defined( _WIN32 )
#   define KS_OS_IS_WIN32           1
#   define KS_OS_IS_WINDOWS         1
#else
#   if defined( unix ) || defined( __unix__ ) || defined( __unix )
#       define KS_OS_IS_UNIX            1
#       if defined( __linux__ )
#           define KS_OS_IS_LINUX       1
#       endif
#   endif
#   if defined( __APPLE__ ) && defined( __MACH__ )
#       undef  KS_OS_IS_UNIX
#       define KS_OS_IS_UNIX            1
#       include <TargetConditionals.h>      // An Appple header.
#       if TARGET_IPHONE_SIMULATOR == 1
#           define  KS_OS_IS_IOS_IN_XCODE_SIMULATOR 1
#           define  KS_OS_IS_IOS                    1
#       elif TARGET_OS_IPHONE == 1
#           define  KS_OS_IS_IOS_ON_IDEVICE         1    // iOS on iPhone, iPad, etc.
#           define  KS_OS_IS_IOS                    1
#       elif TARGET_OS_MAC == 1
#           define  KS_OS_IS_OSX                    1
#       endif
#   endif
#endif

#ifdef KS_OS_IS_UNIX
#   undef   __unix__
#   define  __unix__
#endif

namespace kickstart::system_specific {
    #ifdef KS_OS_IS_WINDOWS
        constexpr bool os_is_windows    = true;
    #else
        constexpr bool os_is_windows    = false;
    #endif
}  // namespace kickstart::system_specific
