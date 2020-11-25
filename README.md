Small exercises should be as simple in C++ as in Python. Well that's impossible but ***this C++ header library can help*** a little. Requires C++17.

Example usage:

\[File [*examples/numeric-input/sum-of-two-numbers.3-with-exception-handler.cpp*](source/examples/numeric-input/sum-of-two-numbers.3-with-exception-handler.cpp)\]:
~~~cpp
#include <kickstart/with_utf8_streams.hpp>
using namespace kickstart::with_utf8_streams;

void cpp_main()
{
    out     << "This program computes the sum of two numbers A and B like "
            << 2.72 << " and " << 3.14 << "."
            << endl;

    out << endl;
    const double    a   = to_double( input( "Number A, please: " ) );
    const double    b   = to_double( input( "Number B, please: " ) );
    
    out << endl;
    out << a << " + " << b << " = " << a + b << "."  << endl;
}

auto main() -> int { return with_exceptions_displayed( cpp_main ); }
~~~

Here

* The **`using namespace kickstart::with_utf8_streams;`** directive is safe, as opposed to `using namespace std;`.

* The `kickstart::with_utf8_streams` namespace provides an **UTF-8** stream-like object called **`out`** + an **`endl`**, corresponding roughly to `cout` and `endl` from the standard library. The `out` stream also supports ANSI escape sequences e.g. for colors. This namespace also provides a few select standard library identifiers, plus this header's stuff.

* **`input`** reads a line from standard input as an UTF-8 encoded string. The main idea is to have the convenience of Python's `input` function.

* **`to_double`** converts a string to `double`, or fails with an exception. Whitespace on ends of the string is ignored, but other extraneous characters cause failure. For example, if a user by mistake  types Norwegian `3,14` instead of `3.14`, then conversion of `3,14` fails. If extraneous characters were ignored (the default in the standard library) then `3,14` would erroneously produce `3`.

* **`with_exceptions_displayed`** invokes the specified main function; catches any exception; and presents its message on the process’ standard error stream.  When an exception occurs `EXIT_FAILURE` is returned, otherwise `EXIT_SUCCESS`.
