
#include <iostream>
#include <vector>
#include <algorithm>

#include <utils/split.hh>
#include <utils/input.hh>


std::vector<int32_t> compute_differences(const std::vector<int32_t>& values) {
    std::vector<int32_t> result;
    for(size_t i = 0; i < values.size() - 1; i++) {
        result.push_back(values[i] - values[i+1]);
    }
    return result;
}

int32_t extrapolate(const std::vector<int32_t>& values) {
    std::vector<std::vector<int32_t>> compute;
    compute.emplace_back(compute_differences(values));
    int32_t res = compute.back()[0];
    while(!std::all_of(compute.back().begin(), compute.back().end(), [](const int32_t i) { return i == 0; })) {
        compute.emplace_back(compute_differences(compute.back()));
        res += compute.back()[0];
    }
    return res + values[0];
}

int32_t extrapolate_sum(const std::vector<std::vector<int32_t>>& values) {
    int32_t res = 0;
    for(const std::vector<int32_t>& v: values) {
        res += extrapolate(v);
    }
    return res;
}


int main() {

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
        std::reverse(values.back().begin(), values.back().end());
    }

    const int32_t sum = extrapolate_sum(values);
    std::cout << "Sum of the next values : " << sum << std::endl;

	return 0;
}
