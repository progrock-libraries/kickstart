#include <kickstart.hpp>
using namespace kickstart::all;

#include <windows.h>

void foo()
{
    out << "Hit the 'any' key, please: ";
    const HANDLE input_handle = ::GetStdHandle( STD_INPUT_HANDLE );
        
    // Empty the input buffer.
    {
        INPUT_RECORD record;
        DWORD n_read;
        while ( ::PeekConsoleInputW( input_handle, &record, 1, &n_read ) and n_read > 0 ) {
            ::ReadConsoleInputW( input_handle, &record, 1, &n_read );
            out << record.EventType << " ";
        }
    }

    // Read a keypress event.
    {
        for( ;; ) {
            INPUT_RECORD record;
            DWORD n_read;
            if (not ::ReadConsoleInputW(input_handle, &record, 1, &n_read)) {
                break;
            }
            out << record.EventType << " ";
            if( (record.EventType & KEY_EVENT) != 0 ) {
                break;
            }
        }
    }
}


auto main() -> int
{
    out << "Hello, world!" << endl;
} 

struct S
{
    ~S()
    {
        //system( "pause" );
        ::MessageBoxW( 0, L"Press the OK button to close the window", L"Program execution finished", MB_ICONINFORMATION | MB_SETFOREGROUND );
    }
};

//const S temp;
