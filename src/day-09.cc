
#include <iostream>
#include <vector>
#include <algorithm>

#include <utils/split.hh>
#include <utils/input.hh>


enum class part { one, two };


std::vector<int32_t> compute_differences(const std::vector<int32_t>& values) {
    std::vector<int32_t> result;
    for(size_t i = 0; i < values.size() - 1; i++) {
        result.push_back(values[i+1] - values[i]);
    }
    return result;
}


template <part Part>
int32_t extrapolate(const std::vector<int32_t>& values);

template <>
int32_t extrapolate<part::one>(const std::vector<int32_t>& values) {
    std::vector<std::vector<int32_t>> compute;
    compute.emplace_back(compute_differences(values));
    int32_t res = values.back() + compute.back().back();
    while(std::adjacent_find( compute.back().begin(), compute.back().end(), std::not_equal_to<>() ) != compute.back().end()) {
        compute.emplace_back(compute_differences(compute.back()));
        res += compute.back().back();
    }
    return res;
}

template <>
int32_t extrapolate<part::two>(const std::vector<int32_t>& values) {
    std::vector<std::vector<int32_t>> compute;
    compute.emplace_back(compute_differences(values));
    while(std::adjacent_find( compute.back().begin(), compute.back().end(), std::not_equal_to<>() ) != compute.back().end()) {
        compute.emplace_back(compute_differences(compute.back()));
    }
    int32_t res = 0;
    for(auto it = compute.rbegin(); it != compute.rend(); ++it) {
        res = it->at(0) - res;
    }
    return values[0] - res;
}


template <part Part>
int32_t extrapolate_sum(const std::vector<std::vector<int32_t>>& values) {
    int32_t res = 0;
    for(const std::vector<int32_t>& v: values) {
        res += extrapolate<Part>(v);
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

template <part Part>
void process() {
    std::vector<std::vector<int32_t>> values;

    while(true) {
        std::optional<std::string> line = read_line();
        if(!line.has_value()) {
            break;
        }
        values.emplace_back();
        for(std::string_view repr: split(line.value(), " ")) {
            values.back().push_back(std::stol(std::string(repr))); // TODO better conversion
        }
    }

    const int32_t sum = extrapolate_sum<Part>(values);
    std::cout << "Sum of the next values : " << sum << std::endl;
}

