
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <utils/split.hh>
#include <utils/input.hh>
#include <utils/format.hh>


enum class part { one, two };


template <part Part>
bool compare_cards(char c1, char c2) {
	if constexpr(Part == part::two) {
		// Nothing is strictly smaller than J
		if(c2 == 'J') {
			return false;
		}
		// J is strictly smaller than anything but itself
		// (the second case has already been taken care of)
		if(c1 == 'J') {
			return true;
		}
	}
	// Letters are stronger than numbers
	if(is_number(c1)) {
		// Numbers can be arranged by lexicographic order
		if(is_number(c2)) {
			return c1 < c2;
		}
		return true;
	}
	if(is_number(c2)) {
		return false;
	}
	if constexpr(Part == part::one) {
		// Now both cards can be any letter
		// If the second card is a J, the only way for the first
		// to be greater is for it to be a T
		if(c2 == 'J') {
			return (c1 == 'T');
		}
		// Knowing that the second card is any letter but J,
		// if the first card is a J it is always smaller,
		// unless the second is a T
		if(c1 == 'J') {
			return (c2 != 'T');
		}
	}
	// Reverse lexicographic order works fine for the rest of the letters
	return c1 > c2;
}


template <part Part>
class card_hand {

public:
	enum class hand_type {
		five_of_a_kind  = 7,
		four_of_a_kind  = 6,
		full_house      = 5,
		three_of_a_kind = 4,
		two_pair        = 3,
		one_pair        = 2,
		high_card       = 1
	};

	card_hand(std::string&& cards, const uint32_t value): 
		_cards(std::move(cards)), _type(compute_type(_cards)), _value(value) {}
	
	uint32_t bid() const { return _value; }
	
	bool operator<(const card_hand<Part>& other) const {
		if(_type != other._type) {
			return static_cast<uint8_t>(_type) < static_cast<uint8_t>(other._type);
		}
		for(size_t i = 0; i < 5; i++) {
			if(_cards[i] != other._cards[i]) {
				return compare_cards<Part>(_cards[i], other._cards[i]);
			}
		}
		return false; // both hands are equal
	}

	template <part P>
	friend std::ostream& operator<<(std::ostream& os, const card_hand<P>& hand);

private:
	std::string _cards;
	hand_type _type;
	uint32_t _value;

	static hand_type compute_type(const std::string& cards) {
		std::unordered_map<char,uint8_t> counter;
		for(const char c: cards) {
			// Note: if the card is not already in the map,
			//       the value is default-initialized to 0
			counter[c]++;
		}
		uint8_t wildcard_count = 0;
		if constexpr(Part == part::two) {
			const auto it = counter.find('J');
			if(it != counter.end() && it->second < 5) {
				wildcard_count += it->second;
				counter.erase(it);
			}
		}
		const auto max_it = std::max_element(
			counter.begin(), counter.end(), 
			[](const std::pair<char,uint8_t>& e1, const std::pair<char,uint8_t>& e2) {
        		return e1.second < e2.second;
    		}
		);
		max_it->second += wildcard_count;
		const uint8_t max_count = max_it->second;
		if(max_count == 5) {
			return hand_type::five_of_a_kind;
		}
		if(max_count == 4) {
			return hand_type::four_of_a_kind;
		}
		if(max_count == 1) {
			return hand_type::high_card;
		}
		if(max_count == 2) {
			return (counter.size() == 4) ? hand_type::one_pair : hand_type::two_pair;
		}
		// max_count == 3
		return (counter.size() == 2) ? hand_type::full_house : hand_type::three_of_a_kind;
	}

};

template <part Part>
std::ostream& operator<<(std::ostream& os, const card_hand<Part>& hand) {
    os << hand._cards << " " << hand._value;
    return os;
}


template <part Part>
uint64_t winnings(std::vector<card_hand<Part>>& hands) {
	uint64_t res = 0;
	std::sort(hands.begin(), hands.end());
	for(size_t i = 0; i < hands.size(); i++) {
		std::cout << "[" << (i+1) << "] " << hands[i] << " -> " << (i+1) * hands[i].bid() << std::endl;
		res += (i+1) * hands[i].bid();
	}
	return res;
}


template <part Part>
void process();

int main(int argc, char** argv) {

	const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);

	if(is_part_2) {
		process<part::two>();
	} else {
		process<part::one>();
	}

	return 0;
}


template<part Part>
void process() {

	std::vector<card_hand<Part>> hands;

    while(true) {
		std::optional<std::string> line = read_line();
		if(!line.has_value()) {
			break;
		}
		if(line->length() > 0) {
			auto [cards, value_repr] = split_once(line.value(), " ");
			hands.emplace_back(std::string(cards), std::stoul(std::string(value_repr))); // TODO better conversion
		}
	}

	const uint64_t total = winnings(hands);
	std::cout << "Total winnings : " << total << std::endl;

}

