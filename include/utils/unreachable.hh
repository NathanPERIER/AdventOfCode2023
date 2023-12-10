
#pragma once

#include <exception>


class unreachable : public std::exception {
public:
    const char* what() {
        return "Reached theorically unreachable code";
    }
};
