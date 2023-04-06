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

char *size2chars(std::size_t i, char s[8]) {
    for (int j = 7; j > -1; j--) {
        s[j] = i & 0b1111;
        i >>= 4;
    }
    return s;
}

std::size_t chars2size(char s[8]) {
    std::size_t i = 0;
    for (int j = 7; j > -1; j--) {
        i += s[j] << ((7 - j) * 4);
    }
    return i;
}

const double NANO = 1000000000.0;
const double MICRO = 1000000.0;
const double Milli = 1000.0;

#endif