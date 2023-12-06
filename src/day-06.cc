
#include <iostream>
#include <vector>
#include <cmath>

#include <utils/split.hh>
#include <utils/format.hh>
#include <utils/input.hh>


struct boat_race {
	uint64_t duration;
	uint64_t record;
};


/*
 |  We know that :
 |    - distance = speed * time_moving
 |    - speed = hold_duration
 |    - time_moving = race_duration - hold_duration
 |    - 0 <= hold_duration <= race_duration
 |
 |  ==> distance = hold_duration * (race_duration - hold_duration)
 |
 |  We assume that :
 |   - record > 0
 |
 |  In order to beat the record, we have to travel a greater distance than the record :
 |
 |      distance > record
 |  <=> hold_duration * (race_duration - hold_duration) > record
 |  <=> (hold_duration * race_duration) - (hold_duration)² - record > 0
 |
 |  Let f be the function : f(t) = -t² + race_duration * t - record
 |
 |  We want to find the values of t for which f(t) > 0
 |
 |  f'(t) = race_duration - 2t
 |
 |  +-------+-------------------------------------------------+
 |  | t     |     0         race_duration/2     race_duration |
 |  +-------+-------------------------------------------------+
 |  | f'(t) |          +           0           -              |
 |  +-------+-------------------------------------------------+
 |  | f     |                   maximum                       |
 |  |       |      increasing              decreasing         |
 |  +-------+-------------------------------------------------+
 |
 |  We can see that the function increases and then decreases.
 |  It can potentially cross the y axis :
 |   - twice (negative -> positive -> negative)
 |   - once (maximum is zero, strictly negative everywhere else)
 |   - 0 times (always strictly negative)
 |
 |  Note: f cannot be always strictly positive since f(0) = f(race_duration) = -record < 0
 |
 |  delta = b² - 4ac
 |        = (race_duration)² - 4 * (-1) * (- record)
 |        = (race_duration)² - 4 * record
 |
 |  Case delta < 0 : f is always negative, there are no solutions
 |
 |  Case delta = 0 :
 |      The only possible solution is t = -b/2a = race_duration/2
 |      This is a solution only if this number is an integer (i.e. only if race_duration is even)
 |      In the other case, there are no solutions
 |
 |  Case delta > 0 :
 |      The function crosses the y axis in two points :
 |       - t1 = (-b + sqrt(delta)) / 2a = (race_duration - sqrt(delta)) / 2
 |       - t2 = (-b - sqrt(delta)) / 2a = (race_duration + sqrt(delta)) / 2
 |
 |      With t1 < t2
 |
 |      Due to the integral nature of the solutions and the shape of the curve,
 |      all points between ceil(t1) and floor(t2) are solutions
 |
 |      Hence, the result set is : { max(0, ceil(t1)), ..., min(race_duration, floor(t2)) }
 |
 |      Since f(0) < 0 and f(race_duration) < 0, we know that 0 < t1 < t2 < race_duration
 |      Hence, we actually don't need to check the boundaries and the result set can be written as : 
 |
 |                                  { ceil(t1), ..., floor(t2) }
 | 
 |      The number of items in this set is : floor(t2) - ceil(t1) + 1
 */

uint64_t compute_number_wins(const boat_race& r) {
	const int64_t delta = static_cast<int64_t>(r.duration * r.duration) - 4 * static_cast<int64_t>(r.record);
	std::cout << "\tdelta = b² - 4ac = " << r.duration << "² - 4 * (" << r.record << ") = " << delta << std::endl;
	if(delta < 0) {
		std::cout << "\tno possible results" << std::endl;
		return 0; // should not actually happen
	}
	if(delta == 0) {
		std::cout << "\tat most one result" << std::endl;
		return 1 - (r.duration & 0x00000001); // 1 if duration is even, else 0
	}
	const double sqrt_delta = std::sqrt(static_cast<double>(delta));
	std::cout << "\tsqrt(delta) = " << sqrt_delta << std::endl;
	const double t1 = (static_cast<double>(r.duration) - sqrt_delta) / 2.0;
	const double t2 = (static_cast<double>(r.duration) + sqrt_delta) / 2.0;
	std::cout << "\tt1 = " << t1 << "ms => " << static_cast<uint64_t>(std::ceil(t1))  << std::endl;
	std::cout << "\tt2 = " << t2 << "ms => " << static_cast<uint64_t>(std::floor(t2)) << std::endl;
	const uint64_t res = static_cast<uint64_t>(std::floor(t2) - std::ceil(t1)) + 1;
	std::cout << "\t ==> " << res << " winning possibilities" << std::endl;
	return res;
}


uint64_t process_races(const std::vector<boat_race>& races) {
	uint64_t res = 1;
	for(const boat_race& r: races) {
		std::cout << "Race (d=" << r.duration << "ms, r=" << r.record << "mm)" << std::endl;
		res *= compute_number_wins(r);
	}
	return res;
}


int main(int argc, char** argv) {
	
	const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);

	std::vector<boat_race> races;

	std::string times     = read_line().value();
	std::string distances = read_line().value();
	
	if(is_part_2) {
		size_t i = 0;
		while(i != times.length()) {
			if(!is_number(times[i])) {
				times.erase(i, 1);
			} else {
				i++;
			}
		}
		i = 0;
		while(i != distances.length()) {
			if(!is_number(distances[i])) {
				distances.erase(i, 1);
			} else {
				i++;
			}
		}
		
		const boat_race r = {
			static_cast<uint64_t>(std::stoull(times)),
			static_cast<uint64_t>(std::stoull(distances))
		};
		std::cout << "Race (d=" << r.duration << "ms, r=" << r.record << "mm)" << std::endl;
		const uint64_t nb_ways = compute_number_wins(r);
		std::cout << "Number of ways to win : " << nb_ways << std::endl;

		return 0;
	}

	// Part 1

	for(const std::string_view repr: split(split_once(times, ": ").second, " ")) {
		if(repr.length() == 0) {
			continue;
		}
		races.push_back({ static_cast<uint32_t>(std::stoul(std::string(repr))), 0 }); // TODO better parsing
	}

	size_t i = 0;
	for(const std::string_view repr: split(split_once(distances, ": ").second, " ")) {
		if(repr.length() == 0) {
			continue;
		}
		races[i].record = std::stoul(std::string(repr)); // TODO better parsing
		i++;
	}


	const uint64_t prod = process_races(races);
	std::cout << "Product of the numbers of ways to win : " << prod << std::endl;

    return 0;
}

