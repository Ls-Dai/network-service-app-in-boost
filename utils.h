#ifndef _UTILS_H_
#define _UTILS_H_

#ifndef _STD_CPP_
#define _STD_CPP_
#include <bits/stdc++.h>
#endif

#define keep_running for (;;)

inline void logInfo(std::string const& s, std::string endline="\n") {
    std::cout << s << endline;
}

std::string input(std::string const& s, std::string endline="\n") {
    std::string ret{};
    logInfo(s, endline);
    std::getline(std::cin, ret);
    return ret;
}

const double NANO = 1000000000.0;
const double MICRO = 1000000.0;
const double Milli = 1000.0;

#endif