#include <kickstart/process/Commandline/module-interface.hpp>

namespace kickstart::process::_definitions {
    inline auto Commandline::singleton() -> const Commandline& { throw 666; }
}  // namespace kickstart::process::_definitions
