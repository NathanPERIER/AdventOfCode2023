
#pragma once

#include <optional>
#include <iostream>


inline std::optional<std::string> read_line() {
	if(std::cin.eof()) {
		return std::nullopt;
	}
	std::string line;
	std::getline(std::cin, line);
	if(line.length() > 0 && line.back() == '\n') {
        line.pop_back();
    }
	return std::make_optional<std::string>(std::move(line));
}

