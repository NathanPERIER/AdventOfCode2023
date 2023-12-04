
#include <iostream>
#include <vector>


bool is_number(char c) {
    return c >= '0' && c <= '9';
}


uint64_t extract_number(std::string& line, size_t pos) {
    if(!is_number(line[pos])) {
        return 0;
    }

    size_t begin = pos;
    while(begin > 0 && is_number(line[begin-1])) {
        begin--;
    }

    size_t end = pos+1;
    while(end < line.length() && is_number(line[end])) {
        end++;
    }

    std::string repr = line.substr(begin, end - begin);
    for(size_t i = begin; i < end; i++) { // clear to avoid reading an integer several times
        line[i] = '.';
    }

    return std::stoull(repr);
}


uint64_t process_lines(std::vector<std::string>& lines) {
    uint64_t res = 0;

    // Note : it is technically possible to do this while only storing
    // two lines and remembering the indexes for the next line
    // Though I was too lazy for that
    for(size_t i = 0; i < lines.size(); i++) {
        std::string& line = lines[i];
        for(size_t j = 0; j < line.size(); j++) {
            if(line[j] == '.' || is_number(line[j])) {
                continue;
            }
            if(j > 0) {
                res += extract_number(line, j-1);
                if(i > 0) {
                    res += extract_number(lines[i-1], j-1);
                }
                if(i < lines.size() - 1) {
                    res += extract_number(lines[i+1], j-1);
                }
            }
            if(j < line.size() - 1) {
                res += extract_number(line, j+1);
                if(i > 0) {
                    res += extract_number(lines[i-1], j+1);
                }
                if(i < lines.size() - 1) {
                    res += extract_number(lines[i+1], j+1);
                }
            }
            if(i > 0) {
                res += extract_number(lines[i-1], j);
            }
            if(i < lines.size() - 1) {
                res += extract_number(lines[i+1], j);
            }
        }
    }

    return res;
}


int main() {

    std::vector<std::string> lines;

    while(true) {
		std::string line;
        std::getline(std::cin, line);
		if(std::cin.eof()) {
			break;
		}
        if(line.length() > 0 && line.back() == '\n') {
            line.pop_back();
        }
        lines.push_back(std::move(line));
	}

    uint64_t sum = process_lines(lines);
    for(auto& line: lines) {
        std::cout << line << std::endl;
    }
    std::cout << "Sum of the part numbers : " << sum << std::endl;

    return 0;
}
