// Example at <url: https://github.com/jarro2783/cxxopts#example>.

#include <iostream>
#include <cxxopts.hpp>

int main(int argc, char** argv)
{
    cxxopts::Options options("test", "A brief description");

    options.add_options()
        ("b,bar", "Param bar", cxxopts::value<std::string>())
        ("d,debug", "Enable debugging", cxxopts::value<bool>()->default_value("false"))
        ("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }
    bool debug = result["debug"].as<bool>();
    std::string bar;
    if (result.count("bar"))
      bar = result["bar"].as<std::string>();
    int foo = result["foo"].as<int>();

    return 0;
}
