
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

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

uint32_t card_matches(const card& c) {
	return std::count_if(c.we_have.begin(), c.we_have.end(), [&c](uint32_t n) {
		return (c.winning.find(n) != c.winning.end());
	});
} 


uint32_t compute_values(const std::vector<card>& cards) {
	uint32_t res = 0;
	for(const card& c: cards) {
		res += card_value(c);
	}
	return res;
}


uint64_t nb_cards(const std::vector<card>& cards) {
	uint64_t sum = 0;
	std::vector<uint32_t> counts(cards.size(), 1);
	for(size_t i = 0; i < cards.size(); i++) {
		const uint32_t nb_matches = card_matches(cards[i]);
		std::cout << "Card " << i+1 << " x" << counts[i] << " : " << nb_matches << " matching number" << (nb_matches != 1 ? "s" : "") << std::endl; 
		for(size_t j = 1; j <= nb_matches && (i+j) < cards.size(); j++) {
			counts[i+j] += counts[i];
			std::cout << "\t-> Card " << i+j+1 << " x" << counts[i+j] << " (+" << counts[i] << ")" << std::endl;
		}
		sum += counts[i];
	}
	return sum;
}


int main(int argc, char** argv) {

	const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);

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
		const auto [_, numbers] = split_once(line, ": ");
		const auto [we_have_repr, winning_repr] = split_once(numbers, " | ");
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

	if(is_part_2) {
		const uint64_t count = nb_cards(cards);
		std::cout << "Number of cards : " << count << std::endl;
	} else {
		const uint32_t sum = compute_values(cards);
		std::cout << "Total points : " << sum << std::endl;
	}

    return 0;
}

