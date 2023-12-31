
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

#include <utils/split.hh>


class range_map {

public:

	range_map(): _entries() {}

	struct entry {
		uint32_t source_start;
		uint32_t destination_start;
		uint32_t length;
	};
	
	uint32_t operator[](const uint32_t val) const {
		for(const entry& e: _entries) {
			if(val >= e.source_start && (val - e.source_start) < e.length) {
				return e.destination_start + (val - e.source_start);
			}
		}
		return val;
	}

	void add_entry(const uint32_t source_start, const uint32_t destination_start, const uint32_t length) {
		_entries.push_back({ source_start, destination_start, length });
	}
	
private:
	std::vector<entry> _entries;

};


uint32_t min_location_pt1(const std::vector<uint32_t>& seeds, const std::vector<range_map>& maps) {
	uint32_t res = 0xffffffff;
	for(uint32_t seed: seeds) {
		for(const range_map& mapping: maps) {
			seed = mapping[seed];
		}
		res = (res < seed) ? res : seed;
	}
	return res;
}

uint32_t min_location_pt2(const std::vector<uint32_t>& seeds, const std::vector<range_map>& maps) {
	uint32_t res = 0xffffffff;
	for(size_t i = 0; i < seeds.size()-1; i += 2) {
		std::cout << "Seed range " << (i/2 + 1) << "/" << (seeds.size()/2 + 1) << " : "
				  << seeds[i] << " -> " << (seeds[i] + seeds[i+1] - 1) << " (" << seeds[i+1] << " elements)" << std::endl;
		#pragma omp parallel for shared(res)
		for(uint32_t j = 0; j < seeds[i+1]; j++) {
			uint32_t seed = seeds[i] + j;
			for(const range_map& mapping: maps) {
				seed = mapping[seed];
			}
			if(seed < res) {
				#pragma omp critical
				res = seed;
			}
		}
	}
	return res;
}


int main(int argc, char** argv) {

	const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);
	const auto min_location = is_part_2 ? min_location_pt2 : min_location_pt1;

	std::vector<uint32_t> seeds;
    std::vector<range_map> maps;

	std::optional<range_map> current_map;

    while(true) {
		std::string line;
        std::getline(std::cin, line);
		if(std::cin.eof()) {
			break;
		}
        if(line.length() > 0 && line.back() == '\n') {
            line.pop_back();
        }
		if(seeds.size() == 0) {
			auto [_, seeds_repr] = split_once(line, ": ");
			for(const std::string_view repr: split(seeds_repr, " ")) {
				seeds.push_back(std::stoul(std::string(repr))); // TODO better parsing
			}
		}
		if(line.length() == 0 && current_map.has_value()) {
			maps.push_back(std::move(current_map.value()));
			current_map = std::nullopt;
			continue;
		}
		if(!current_map.has_value()) {
			current_map.emplace();
			continue;
		}
		split split_line(line, " ");
		const std::vector<std::string_view> split_entry(split_line.begin(), split_line.end());
		current_map->add_entry( // TODO better parsing
			std::stoul(std::string(split_entry[1])),
			std::stoul(std::string(split_entry[0])),
			std::stoul(std::string(split_entry[2]))
		);
	}

	if(current_map.has_value()) {
		maps.push_back(std::move(current_map.value()));
		current_map = std::nullopt;
	}

	const uint32_t min = min_location(seeds, maps);
	std::cout << "Minimum location : " << min << std::endl;

    return 0;
}

