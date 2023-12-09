
#pragma once

#include <iostream>
#include <vector>


template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& list) {
    os << '[';
    auto it = list.begin();
    if(it != list.end()) {
        os << *it;
        ++it;
        while(it != list.end()) {
            os << ", " << *it;
            ++it;
        }
    }
    os << ']';
    return os;
}
