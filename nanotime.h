// The MIT License (MIT)

// Copyright (c) 2017 Daniel Feist

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef __NANOTIME_H_
#define __NANOTIME_H_

#include <iostream>
#include <chrono>
#include <algorithm>
#include <unistd.h>
#include <stdexcept>
#include <vector>
#include <sys/wait.h>

class nanotime
{
private:
    std::chrono::nanoseconds dur{0};
    std::vector<std::string> input;
public:
    template<class... Ts>
    explicit nanotime(Ts&&... args)
    {
        static_assert((std::is_convertible<Ts, std::string>::value &&...), "args must be convertible to std::string!");
        int dummy[] = { 0, (input.emplace_back(std::forward<Ts>(args)), 0)... };
        (void)dummy;
    }

    template<class T, typename = typename std::enable_if<std::is_same<T, std::vector<std::string>>::value>>
    explicit nanotime(T&& vec)
    : input(std::forward<T>(vec))
    { ; }

    nanotime(nanotime&&) = default;
    nanotime& operator=(nanotime&&) = default;

    nanotime(const nanotime&) = default;
    nanotime& operator=(const nanotime&) = default;

    inline auto get_duration() const noexcept { return dur; }

    void execute();
};

void nanotime::execute()
{
    pid_t child_pid;
    int status;

    std::vector<const char*> argv(input.size());
    std::transform(std::cbegin(input), std::cend(input), argv.begin(),
                   [](const auto& e) { return e.c_str(); });

    argv.push_back(nullptr);

    using namespace std::chrono;
    high_resolution_clock::time_point start = high_resolution_clock::now();

    child_pid = fork();

    if (child_pid == -1)
	    throw std::runtime_error{"fork() failed"};

    if (child_pid == 0) // executed by child
    {
        if (execv(argv[0], const_cast<char**>(&argv[1])) == -1)
            throw std::runtime_error{"execv() failed"};
    }
    else // executed by parent
    {
        if ((child_pid = wait(&status)) == -1)
        {
            throw std::runtime_error{"wait() failed"};
        }

        if (WIFEXITED(status))
        {
            auto end = high_resolution_clock::now();
            dur = end - start;
            //std::cout << "child exited normally with status: " << WEXITSTATUS(status) << "\n";
        }
        else if (WIFSIGNALED(status))
        {
            throw std::runtime_error{"child exited due to uncaught signal: " + std::to_string(WTERMSIG(status))};
        }
        else if (WIFSTOPPED(status))
        {
            throw std::runtime_error{"child stopped due to signal: " + std::to_string(WSTOPSIG(status))};

        #ifdef WIFCONTINUED
        } else if (WIFCONTINUED(status)) {
                 std::cout << "child continued\n" << "\n";
        #endif

        }
        else
        {
            throw std::runtime_error{"child has unexpected status: " + std::to_string(status)};
        }
    }
} // execute()

#endif // header guard
