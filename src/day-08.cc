
#include <iostream>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <vector>

#include <utils/split.hh>
#include <utils/input.hh>


class lazy_node {

public:
	lazy_node(std::string&& n, std::string&& l, std::string&& r): 
		_name(std::move(n)), _left_name(std::move(l)), _right_name(std::move(r)), _left(nullptr), _right(nullptr) {}

	std::shared_ptr<lazy_node> go(char lr, const std::unordered_map<std::string,std::shared_ptr<lazy_node>>& nodes) {
		if(lr == 'L') {
			if(_left == nullptr) {
				_left = nodes.at(_left_name);
			}
			return _left;
		}
		if(_right == nullptr) {
			_right = nodes.at(_right_name);
		}
		return _right;
	}

	const std::string& name() const { return _name; }

private:
	const std::string _name;
	const std::string _left_name;
	const std::string _right_name;

	std::shared_ptr<lazy_node> _left;
	std::shared_ptr<lazy_node> _right;

};


uint32_t compute_steps(const std::unordered_map<std::string,std::shared_ptr<lazy_node>>& nodes, 
			const std::string& directions, const std::string& start, const std::string& destination) {
	uint32_t res = 0;
	size_t pos = 0;
	std::shared_ptr<lazy_node> n = nodes.at(start);
	while(n->name() != destination) {
		std::cout << n->name() << " --(" << directions[pos] << ")--> ";
		n = n->go(directions[pos], nodes);
		res++;
		pos++;
		if(pos >= directions.size()) {
			pos = 0;
		}
		std::cout << n->name() << " (d=" << res << ")" << std::endl;
	}
	return res;
}


int main() {

	std::unordered_map<std::string,std::shared_ptr<lazy_node>> nodes;

	std::string directions = read_line().value();
	read_line();

    while(true) {
		std::optional<std::string> line = read_line();
		if(!line.has_value()) {
			break;
		}
		if(line.value().length() == 0) {
			continue;
		}
		auto [name, directions_rep] = split_once(line.value(), " = ");
		auto [left_name, right_name] = split_once(directions_rep.substr(1, directions_rep.length() - 2), ", ");
		nodes[std::string(name)] = std::make_shared<lazy_node>(std::string(name), std::string(left_name), std::string(right_name));
	}

	const uint32_t nb_steps = compute_steps(nodes, directions, "AAA", "ZZZ");
	std::cout << "Number of steps : " << nb_steps << std::endl;

	return 0;
}

