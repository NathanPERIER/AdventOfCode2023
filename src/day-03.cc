
#include <iostream>
#include <vector>
#include <algorithm>


bool is_number(char c) {
    return c >= '0' && c <= '9';
}


std::pair<size_t,size_t> find_number_range(const std::string& line, size_t pos) {
    size_t begin = pos;
    while(begin > 0 && is_number(line[begin-1])) {
        begin--;
    }

    size_t end = pos+1;
    while(end < line.length() && is_number(line[end])) {
        end++;
    }

    return { begin, end };
}



/*-----------------+
|  First Part      |
+------------------*/


uint64_t extract_number(std::string& line, size_t pos) {
    if(!is_number(line[pos])) {
        return 0;
    }

    const auto [begin, end] = find_number_range(line, pos);

    std::string repr = line.substr(begin, end - begin);
    for(size_t i = begin; i < end; i++) { // clear to avoid reading an integer several times
        line[i] = '.';
    }

    return std::stoull(repr);
}


uint64_t process_lines_pt1(std::vector<std::string>& lines) {
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



/*-----------------+
|  Second Part      |
+------------------*/


uint64_t parse_number(const std::string& line, size_t pos, uint8_t& success_count) {
    if(!is_number(line[pos])) {
        return 1;
    }

    success_count++; // usually a bad idea but extremely convenient here
    const auto [begin, end] = find_number_range(line, pos);

    std::string repr = line.substr(begin, end - begin);
    return std::stoull(repr);
}


uint64_t process_lines_pt2(std::vector<std::string>& lines) {
    uint64_t res = 0;

    for(size_t i = 0; i < lines.size(); i++) {
        const std::string& line = lines[i];
        for(size_t j = 0; j < line.size(); j++) {
            if(line[j] != '*') {
                continue;
            }
            uint8_t number_count = 0;
            uint64_t ratio = 1;
            // I'm sure there's a better way to do this...
            if(i > 0) {
                if(is_number(lines[i-1][j])) {
                    ratio *= parse_number(lines[i-1], j, number_count);
                } else {
                    if(j > 0) {
                        ratio *= parse_number(lines[i-1], j-1, number_count);
                    }
                    if(j < line.size() - 1) {
                        ratio *= parse_number(lines[i-1], j+1, number_count);
                    }
                }
            }
            if(i < lines.size() - 1) {
                if(is_number(lines[i+1][j])) {
                    ratio *= parse_number(lines[i+1], j, number_count);
                } else {
                    if(j > 0) {
                        ratio *= parse_number(lines[i+1], j-1, number_count);
                    }
                    if(j < line.size() - 1) {
                        ratio *= parse_number(lines[i+1], j+1, number_count);
                    }
                }
            }
            if(j > 0) {
                ratio *= parse_number(line, j-1, number_count);
            }
            if(j < line.size() - 1) {
                ratio *= parse_number(line, j+1, number_count);
            }
            if(number_count == 2) {
                res += ratio;
            }
        }
    }

    return res;
}



int main(int argc, char** argv) {

    const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);
	const auto process_lines = is_part_2 ? process_lines_pt2 : process_lines_pt1;

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
    std::cout << "Sum of the part numbers : " << sum << std::endl;

    return 0;
}
