#include <iostream>
#include "ini_parser.hpp"

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        ini::IniParser parser = ini::IniParser::from_file(arg);
        std::cout << parser.to_string() << '\n';
    }

    return 0;
}