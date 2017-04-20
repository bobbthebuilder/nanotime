#include "nanotime.h"

#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        /// example usage (1) - variadic template ctor of universal references

        nanotime cmd1("/usr/bin/ls", "ls", "-la");
        cmd1.execute();

        auto d1 = cmd1.get_duration();
        std::cout << "duration cmd1: " << d1.count() << "ns\n";
        //std::cout << "duration cmd1: " << std::chrono::duration_cast<std::chrono::microseconds>(d1).count() << "us\n";

        /// example usage (2) - ctor of universal reference to vector-of-strings

        std::vector<std::string> cmd2_vec {"/usr/bin/g++", "g++", "-o", "test", "-std=c++1z", "example.cpp"};
        nanotime cmd2(cmd2_vec);
        cmd2.execute();

        auto d2 = cmd2.get_duration();
        std::cout << "duration cmd2: " << d2.count() << "ns\n";

        /// example usage (3) - ctor of universal reference to vector-of-strings

        /*
        std::vector<std::string> cmd3_vec(argv + 1, argv + argc);
        nanotime cmd3(std::move(cmd3_vec));
        cmd3.execute();

        auto d3 = cmd3.get_duration();
        std::cout << "duration cmd2: " << d3.count() << "ns\n";
        */
    }
    catch (const std::exception& e) { std::cout << e.what() << "\n"; }
    catch (...) { std::cout << "Unknown exception\n"; }

    return 0;
}
