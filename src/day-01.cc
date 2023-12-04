
#include <iostream>
#include <algorithm>


uint64_t calibration_value(const std::string& repr) {
	static const auto is_number = [](const char c) { return (c >= '0' && c <= '9'); };
	auto it_first = std::find_if(repr.begin(),  repr.end(),  is_number);
	auto it_last  = std::find_if(repr.rbegin(), repr.rend(), is_number);
	const std::string val = { *it_first, *it_last };
	return std::stoull(val);
}

int main() {
	
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

