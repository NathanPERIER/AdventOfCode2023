
#include <vector>
#include <iostream>
#include <algorithm>


uint64_t calibration_value_pt1(const std::string& repr) {
	static const auto is_number = [](const char c) { return (c >= '0' && c <= '9'); };
	const auto it_first = std::find_if(repr.begin(),  repr.end(),  is_number);
	const auto it_last  = std::find_if(repr.rbegin(), repr.rend(), is_number);
	return static_cast<uint64_t>( (*it_first - '0') * 10 + (*it_last - '0') );
}


const std::vector<std::pair<std::string,uint8_t>> digits = {
	{ "one",   1 }, 
	{ "two",   2 },
	{ "three", 3 },
	{ "four",  4 },
	{ "five",  5 },
	{ "six",   6 },
	{ "seven", 7 },
	{ "eight", 8 },
	{ "nine",  9 }
};

uint64_t calibration_value_pt2(const std::string& repr) {
	static const auto is_number = [](const char c) { return (c >= '0' && c <= '9'); };
	const auto it_first = std::find_if(repr.begin(),  repr.end(),  is_number);
	const auto it_last  = std::find_if(repr.rbegin(), repr.rend(), is_number);

	uint8_t first = 0;
	uint64_t first_idx = repr.length() - 1;
	if(it_first != repr.end()) {
		first = *it_first - '0';
		first_idx = std::distance(repr.begin(), it_first);
	}

	uint8_t last = 0;
	uint64_t last_idx = 0;
	if(it_last != repr.rend()) {
		last  = *it_last  - '0';
		last_idx  = repr.length() - 1 - std::distance(repr.rbegin(), it_last);
	}

	for(const auto& d: digits) {
		size_t idx = repr.find(d.first);
		if(idx != std::string::npos && idx < first_idx) {
			first_idx = idx;
			first = d.second;
			if(idx == 0) {
				break;
			}
		}
	}

	for(const auto& d: digits) {
		size_t idx = repr.rfind(d.first);
		if(idx != std::string::npos && idx > last_idx) {
			last_idx = idx;
			last = d.second;
			if(idx == repr.length() - 1) {
				break;
			}
		}
	}

	return static_cast<uint64_t>( first * 10 + last );
}


int main(int argc, char** argv) {
	
	const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);
	const auto calibration_value = is_part_2 ? calibration_value_pt2 : calibration_value_pt1;

	uint64_t total = 0;
	while(true) {
		std::string line;
		std::cin >> line;
		if(std::cin.eof()) {
			break;
		}
		total += calibration_value(line);
	}
 
	std::cout << "Sum of the calibration values : " << total << std::endl;

	return 0;
}

