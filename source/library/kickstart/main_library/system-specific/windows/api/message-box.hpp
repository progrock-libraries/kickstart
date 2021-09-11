// Source encoding: utf-8  --  π is (or should be) a lowercase greek pi.
#pragma once
#include <kickstart/main_library/system-specific/windows/api/~header-boilerplate-stuff.hpp>

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

#include <kickstart/main_library/system-specific/windows/api/types.hpp>

namespace kickstart::winapi::_definitions {
    using namespace kickstart::language;        // Type_ etc.

    // Visual C++ 2019 (16.3.3) and later may issue errors on the Windows API function
    // declarations here when <windows.h> is also included, as explained in
    //
    // “Including Windows.h and Boost.Interprocess headers leads to C2116 and C2733”
    // https://developercommunity.visualstudio.com/content/problem/756694/including-windowsh-and-boostinterprocess-headers-l.html
    //
    // A fix is to use the per October 2019 undocumented option “/Zc:externC-”.
    //
    // A more fragile fix is to include <windows.h> BEFORE any Kickstart header, or
    // to define KS_USE_WINDOWS_H or BOOST_USE_WINDOWS_H or both in the build.

    #ifdef MessageBox       // <windows.h> has been included
        using   ::MessageBoxW;

        const UINT mb_abortretryignore          = MB_ABORTRETRYIGNORE;      // 0x00000002L
        //The message box contains three push buttons: Abort, Retry, and Ignore.

        const UINT mb_canceltrycontinue         = MB_CANCELTRYCONTINUE;     // 0x00000006L
        //The message box contains three push buttons: Cancel, Try Again, Continue. Use this message box type instead of MB_ABORTRETRYIGNORE.

        const UINT mb_help                      = MB_HELP;                  // 0x00004000L
        //Adds a Help button to the message box. When the user clicks the Help button or presses F1, the system sends a WM_HELP message to the owner.

        const UINT mb_ok                        = MB_OK;                    // 0x00000000L
        //The message box contains one push button: OK. This is the default.

        const UINT mb_okcancel                  = MB_OKCANCEL;              // 0x00000001L
        //The message box contains two push buttons: OK and Cancel.

        const UINT mb_retrycancel               = MB_RETRYCANCEL;           // 0x00000005L
        //The message box contains two push buttons: Retry and Cancel.

        const UINT mb_yesno                     = MB_YESNO;                 // 0x00000004L
        //The message box contains two push buttons: Yes and No.

        const UINT mb_yesnocancel               = MB_YESNOCANCEL;           // 0x00000003L
        //The message box contains three push buttons: Yes, No, and Cancel.

        const UINT mb_iconexclamation           = MB_ICONEXCLAMATION;       // 0x00000030L
        //An exclamation-point icon appears in the message box.

        const UINT mb_iconwarning               = MB_ICONWARNING;           // 0x00000030L
        //An exclamation-point icon appears in the message box.

        const UINT mb_iconinformation           = MB_ICONINFORMATION;       // 0x00000040L
        //An icon consisting of a lowercase letter i in a circle appears in the message box.

        const UINT mb_iconasterisk              = MB_ICONASTERISK;          // 0x00000040L
        //An icon consisting of a lowercase letter i in a circle appears in the message box.

        const UINT mb_iconquestion              = MB_ICONQUESTION;          // 0x00000020L
        //A question-mark icon appears in the message box. The question-mark message icon is no longer recommended because it does not clearly represent a specific type of message and because the phrasing of a message as a question could apply to any message type. In addition, users can confuse the message symbol question mark with Help information. Therefore, do not use this question mark message symbol in your message boxes. The system continues to support its inclusion only for backward compatibility.

        const UINT mb_iconstop                  = MB_ICONSTOP;              // 0x00000010L
        //A stop-sign icon appears in the message box.

        const UINT mb_iconerror                 = MB_ICONERROR;             // 0x00000010L
        //A stop-sign icon appears in the message box.

        const UINT mb_iconhand                  = MB_ICONHAND;              // 0x00000010L
        //A stop-sign icon appears in the message box.

        const UINT mb_defbutton1                = MB_DEFBUTTON1;            // 0x00000000L
        //The first button is the default button.
        //MB_DEFBUTTON1 is the default unless MB_DEFBUTTON2, MB_DEFBUTTON3, or MB_DEFBUTTON4 is specified.

        const UINT mb_defbutton2                = MB_DEFBUTTON2;            // 0x00000100L
        //The second button is the default button.

        const UINT mb_defbutton3                = MB_DEFBUTTON3;            // 0x00000200L
        //The third button is the default button.

        const UINT mb_defbutton4                = MB_DEFBUTTON4;            // 0x00000300L
        //The fourth button is the default button.

        const UINT mb_applmodal                 = MB_APPLMODAL;             // 0x00000000L
        const UINT mb_systemmodal               = MB_SYSTEMMODAL;           // 0x00001000L
        const UINT mb_taskmodal                 = MB_TASKMODAL;             // 0x00002000L

        const UINT mb_default_desktop_only      = MB_DEFAULT_DESKTOP_ONLY;  // 0x00020000L
        const UINT mb_right                     = MB_RIGHT;                 // 0x00080000L
        const UINT mb_rtlreading                = MB_RTLREADING;            // 0x00100000L
        const UINT mb_setforeground             = MB_SETFOREGROUND;         // 0x00010000L
        const UINT mb_topmost                   = MB_TOPMOST;               // 0x00040000L
        const UINT mb_service_notification      = MB_SERVICE_NOTIFICATION;  // 0x00200000L

    #else
        using namespace kickstart::winapi;
        const UINT mb_abortretryignore          = 0x00000002L;
        //The message box contains three push buttons: Abort, Retry, and Ignore.

        const UINT mb_canceltrycontinue         = 0x00000006L;
        //The message box contains three push buttons: Cancel, Try Again, Continue. Use this message box type instead of MB_ABORTRETRYIGNORE.

        const UINT mb_help                      = 0x00004000L;
        //Adds a Help button to the message box. When the user clicks the Help button or presses F1, the system sends a WM_HELP message to the owner.

        const UINT mb_ok                        = 0x00000000L;
        //The message box contains one push button: OK. This is the default.

        const UINT mb_okcancel                  = 0x00000001L;
        //The message box contains two push buttons: OK and Cancel.

        const UINT mb_retrycancel               = 0x00000005L;
        //The message box contains two push buttons: Retry and Cancel.

        const UINT mb_yesno                     = 0x00000004L;
        //The message box contains two push buttons: Yes and No.

        const UINT mb_yesnocancel               = 0x00000003L;
        //The message box contains three push buttons: Yes, No, and Cancel.

        const UINT mb_iconexclamation           = 0x00000030L;
        //An exclamation-point icon appears in the message box.

        const UINT mb_iconwarning               = 0x00000030L;
        //An exclamation-point icon appears in the message box.

        const UINT mb_iconinformation           = 0x00000040L;
        //An icon consisting of a lowercase letter i in a circle appears in the message box.

        const UINT mb_iconasterisk              = 0x00000040L;
        //An icon consisting of a lowercase letter i in a circle appears in the message box.

        const UINT mb_iconquestion              = 0x00000020L;
        //A question-mark icon appears in the message box. The question-mark message icon is no longer recommended because it does not clearly represent a specific type of message and because the phrasing of a message as a question could apply to any message type. In addition, users can confuse the message symbol question mark with Help information. Therefore, do not use this question mark message symbol in your message boxes. The system continues to support its inclusion only for backward compatibility.

        const UINT mb_iconstop                  = 0x00000010L;
        //A stop-sign icon appears in the message box.

        const UINT mb_iconerror                 = 0x00000010L;
        //A stop-sign icon appears in the message box.

        const UINT mb_iconhand                  = 0x00000010L;
        //A stop-sign icon appears in the message box.

        const UINT mb_defbutton1                = 0x00000000L;
        //The first button is the default button.
        //MB_DEFBUTTON1 is the default unless MB_DEFBUTTON2, MB_DEFBUTTON3, or MB_DEFBUTTON4 is specified.

        const UINT mb_defbutton2                = 0x00000100L;
        //The second button is the default button.

        const UINT mb_defbutton3                = 0x00000200L;
        //The third button is the default button.

        const UINT mb_defbutton4                = 0x00000300L;

        const UINT mb_applmodal                 = 0x00000000L;
        const UINT mb_systemmodal               = 0x00001000L;
        const UINT mb_taskmodal                 = 0x00002000L;

        const UINT mb_default_desktop_only      = 0x00020000L;
        const UINT mb_right                     = 0x00080000L;
        const UINT mb_rtlreading                = 0x00100000L;
        const UINT mb_setforeground             = 0x00010000L;
        const UINT mb_topmost                   = 0x00040000L;
        const UINT mb_service_notification      = 0x00200000L;

        extern "C" auto __stdcall MessageBoxW(
            HWND                hWnd,
            const wchar_t*      lpText,
            const wchar_t*      lpCaption,
            UINT    uType
            ) -> int;
    #endif

    //----------------------------------------------------------- @exported:
    namespace d = _definitions;
    namespace exported_names { using
        d::mb_abortretryignore,
        d::mb_canceltrycontinue,
        d::mb_help,
        d::mb_ok,
        d::mb_okcancel,
        d::mb_retrycancel,
        d::mb_yesno,
        d::mb_yesnocancel,
        d::mb_iconexclamation,
        d::mb_iconwarning,
        d::mb_iconinformation,
        d::mb_iconasterisk,     
        d::mb_iconquestion,
        d::mb_iconstop,
        d::mb_iconerror,
        d::mb_iconhand,
        d::mb_defbutton1,
        d::mb_defbutton2,
        d::mb_defbutton3,
        d::mb_defbutton4,
        d::mb_applmodal,
        d::mb_systemmodal,
        d::mb_taskmodal,
        d::mb_default_desktop_only,
        d::mb_right,
        d::mb_rtlreading,
        d::mb_setforeground,
        d::mb_topmost,
        d::mb_service_notification,
        d::MessageBoxW;
    }  // namespace exported names
}  // namespace kickstart::winapi::_definitions

namespace kickstart::winapi { using namespace _definitions::exported_names; }
