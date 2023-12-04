
#include <iostream>
#include <vector>
#include <numeric>

#include <utils/split.hh>


struct cubes {
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;

    uint64_t power() {
        return red * green * blue;
    }
};


template <uint32_t red, uint32_t green, uint32_t blue>
bool is_possible(const std::vector<cubes>& game) {
    return std::all_of(game.begin(), game.end(), [](const cubes& cb) {
        return cb.red <= red && cb.green <= green && cb.blue <= blue;
    });
}

uint64_t part1(const std::pair<uint32_t,std::vector<cubes>>& game) {
    if(is_possible<12, 13, 14>(game.second)) {
        return game.first;
    }
    return 0;
}


uint64_t part2(const std::pair<uint32_t,std::vector<cubes>>& game) {
    cubes max;
    for(const cubes& cb: game.second) {
        if(cb.red > max.red) {
            max.red = cb.red;
        }
        if(cb.green > max.green) {
            max.green = cb.green;
        }
        if(cb.blue > max.blue) {
            max.blue = cb.blue;
        }
    }
    std::cout << "Min for game " << game.first << " : " << max.red << " red, " << max.green << " green, " << max.blue << " blue, " << std::endl;
    return max.power();
}


int main(int argc, char** argv) {

    const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);
	const auto compute = is_part_2 ? part2 : part1;

    std::vector<std::pair<uint32_t,std::vector<cubes>>> games;

    while(true) {
		std::string line;
        std::getline(std::cin, line);
		if(std::cin.eof()) {
			break;
		}
        if(line.length() > 0 && line.back() == '\n') {
            line.pop_back();
        }
        const auto [pre, list] = split_once(line, ": ");
        const auto [_, num_repr] = split_once(pre, " ");
        const uint32_t game_num = std::stoul(std::string(num_repr)); // TODO better conversion
        std::vector<cubes> game;
        for(const std::string_view grab: split(list, "; ")) {
            cubes cb;
            for(const std::string_view nb_cubes: split(grab, ", ")) {
                auto [count_repr, colour] = split_once(nb_cubes, " ");
                const uint32_t count = std::stoul(std::string(count_repr)); // TODO better conversion
                if(colour == "red") {
                    cb.red += count;
                } else if(colour == "green") {
                    cb.green += count;
                } else { // colour == "blue"
                    cb.blue += count;
                }
            }
            game.push_back(cb);
        }
        games.push_back({ game_num, std::move(game) });
	}

    uint64_t sum = 0;
    for(const auto& game: games) {
        sum += compute(game);
    }
    std::cout << "Sum of the game IDs : " << sum << std::endl;

    return 0;
}
