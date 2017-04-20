# nanotime

`nanotime` a simple command.  

simple utility class to accurately measure the runtime of a simple command in nanoseconds.

## Example Usage

1. download `nanotime.h`
2. create file `test.cpp`:

``` cpp
#include "nanotime.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    try
    {
        std::vector<std::string> cmd3_vec(argv + 1, argv + argc);
        nanotime cmd3(std::move(cmd3_vec)); // constructor of universal reference to vector-of-strings
        cmd3.execute();

        auto d3 = cmd3.get_duration();
        std::cout << "duration cmd2: " << d3.count() << "ns\n";
    }
    catch (const std::exception& e) { std::cout << e.what() << "\n"; }
    catch (...) { std::cout << "Unknown exception\n"; }

    return 0;
 }
 ```

3. compile: `g++ -o test -std=c++1z test.cpp`
4. execute: `./test /usr/bin/ls ls -la`

see `example.cpp` for more details.

## Dependencies

none.

## License 

(c) 2017 Daniel Feist. Licensed under the MIT license.
