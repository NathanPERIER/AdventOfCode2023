
#include <iostream>
#include <vector>
#include <unordered_set>

#include <utils/split.hh>


struct card {
	std::unordered_set<uint32_t> we_have; 
	std::unordered_set<uint32_t> winning; 
};


uint32_t card_value(const card& c) {
	uint32_t res = 0;
	for(uint32_t n: c.we_have) {
		if(c.winning.find(n) != c.winning.end()) {
			res = (res == 0) ? 1 : (res << 1);
		}
	}
	return res;
}


uint32_t compute_values(std::vector<card> cards) {
	uint32_t res = 0;
	for(const card& c: cards) {
		res += card_value(c);
	}
	return res;
}


int main() {

    std::vector<card> cards;

    while(true) {
		std::string line;
        std::getline(std::cin, line);
		if(std::cin.eof()) {
			break;
		}
        if(line.length() > 0 && line.back() == '\n') {
            line.pop_back();
        }
		card c;
		auto [_, numbers] = split_once(line, ": ");
		auto [we_have_repr, winning_repr] = split_once(numbers, " | ");
		for(const std::string_view repr: split(we_have_repr, " ")) {
			if(repr.length() == 0) { continue; }
			c.we_have.insert(std::stoul(std::string(repr))); // TODO better conversion
		}
		for(const std::string_view repr: split(winning_repr, " ")) {
			if(repr.length() == 0) { continue; }
			c.winning.insert(std::stoul(std::string(repr))); // TODO better conversion
		}
        cards.push_back(std::move(c));
	}

    uint64_t points = compute_values(cards);
    std::cout << "Total points : " << points << std::endl;

    return 0;
}

