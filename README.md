Small exercises should be as simple in C++ as in Python.

Well that's impossible but ***this C++ 17 header library can help***.

There’s a separate “Hello, world!”-document with [detailed installation instructions](source/examples/hello-world.md).

---

Example usage:

~~~cpp
#include <kickstart/all.hpp>
using namespace kickstart::all;

void cpp_main()
{
    out << "This program computes the sum of two numbers A and B like "
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

In [this code](source/examples/numeric-io/input/sum-of-two-numbers.3-with-exception-handler.cpp):

* The **`using namespace kickstart::all;`** directive is relatively safe, as opposed to `using namespace std;`.

* The `kickstart::all` namespace provides an **UTF-8** stream-like object called **`out`** + an **`endl`**, corresponding roughly to `cout` and `endl` from the standard library. The `out` stream also supports [ANSI escape sequences](source/examples/console/colored-text.cpp#L9-L13) e.g. for [**colors**](source/library/console/ansi_escape_seq.hpp#L102-L149). And this namespace provides [a few select standard library identifiers](source/library/core/language/stdlib/basics.hpp#L42-L61), plus most all the rest of Kickstart.

* **`input`** reads a line from standard input as an UTF-8 encoded string.

* **`to_double`** converts a string to `double`, or fails with an exception. ASCII whitespace on the ends of the string is ignored, but other extraneous characters cause failure. For example, if a user by mistake  types Norwegian `3,14` instead of `3.14`, then conversion of `3,14` fails.

* **`with_exceptions_displayed`** invokes the specified main function; catches any exception; and presents the outer exception message, if any, on the process’ standard error stream.  When an exception occurs `EXIT_FAILURE` is returned, otherwise `EXIT_SUCCESS`.

---

Some rationales & explanations:

* If extraneous characters were ignored then text input `3,14` would erroneously convert to number `3`. That’s unfortunately the default in the standard library. Kickstart fixes that for you.

* It can be argued that Windows’ `E_FAIL` should be used in Windows because the common C and C++ `EXIT_FAILURE` value, namely 1, is very ambiguous in Windows. Namely, in Windows value 1 as process exit code can mean (1) `ERROR_INVALID_FUNCTION`, or (2) the “HRESULT” `S_FALSE`, or (3) a general failure from a C or C++ program. However, the [principle of least surprise](https://en.wikipedia.org/wiki/Principle_of_least_astonishment) is a roughly equally strong argument for `EXIT_FAILURE`: a beginner learning from examples in a book will expect `EXIT_FAILURE`, the value 1.

* The `<<` notation for output is merely *similar* to the iostreams notation, and internally directs output to the C `FILE*` standard streams `stdout` and `stderr`. More advanced iostreams features such as e.g. manipulators `setprecision` or `setw` are not supported, and there is no corresponding `>>` for input. This makes it possible to provide efficient console i/o with UTF-8 support in Windows. However, iostreams can be used with Kickstart. Iostreams headers are not included by default because there’s a possible cost of increased size of the executable.
