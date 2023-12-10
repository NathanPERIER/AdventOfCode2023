
#include <iostream>
#include <vector>
#include <algorithm>

#include <utils/split.hh>
#include <utils/input.hh>
#include <utils/unreachable.hh>


enum class displacement {
    top,
    right,
    bottom,
    left
};

struct coordinate {
    size_t x;
    size_t y;

    void apply(const displacement d) {
        switch(d) {
            case displacement::top:    y--; break;
            case displacement::right:  x++; break;
            case displacement::bottom: y++; break;
            case displacement::left:   x--; break;
            default: throw unreachable();
        }
    }

    bool operator!=(const coordinate& other) const {
        return (x != other.x) || (y != other.y);
    }
};

coordinate operator+(const coordinate& c, const displacement d) {
    coordinate res = c;
    res.apply(d);
    return res;
}

coordinate& operator+=(coordinate& c, const displacement d) {
    c.apply(d);
    return c;
}

std::ostream& operator<<(std::ostream& out, const coordinate& c) {
    out << '(' << c.x << ", " << c.y << ')';
    return out;
}

std::ostream& operator<<(std::ostream& out, const displacement d) {
    switch(d) {
        case displacement::top:    out << "top";    break;
        case displacement::right:  out << "right";  break;
        case displacement::bottom: out << "bottom"; break;
        case displacement::left:   out << "left";   break;
        default: throw unreachable();
    }
    return out;
}



coordinate get_start(const std::vector<std::string>& layout) {
    coordinate res = {0, 0};
    for(const std::string& line: layout) {
        const size_t pos = line.find('S');
        if(pos != std::string::npos) {
            res.x = pos;
            return res;
        }
        res.y++;
    }
    throw unreachable();
}

displacement step_after_start(const coordinate& start, const std::vector<std::string>& layout) {
    if((start.x > 0)) {
        const char left_tile = layout[start.y][start.x-1];
        if(left_tile == '-' || left_tile == 'F' || left_tile == 'L') {
            return displacement::left;
        }
    }
    if(start.x < layout[start.y].size() - 1) {
        const char right_tile = layout[start.y][start.x+1];
        if(right_tile == '-' || right_tile == '7' || right_tile == 'J') {
            return displacement::right;
        }
    }
    if(start.y > 0) {
        const char top_tile = layout[start.y-1][start.x];
        if(top_tile == '|' || top_tile == '7' || top_tile == 'F') {
            return displacement::top;
        }
    }
    if(start.y < layout.size() - 1) {
        const char bottom_tile = layout[start.y+1][start.x];
        if(bottom_tile == '|' || bottom_tile == 'J' || bottom_tile == 'L') {
            return displacement::bottom;
        }
    }
    throw unreachable();
}

displacement next_displacement(const coordinate& current_pos, const std::vector<std::string>& layout, displacement prev_displ) {
    const char current_tile = layout[current_pos.y][current_pos.x];
    if(current_tile == '|') {
        return (prev_displ == displacement::top) ? displacement::top : displacement::bottom;
    }
    if(current_tile == '-') {
        return (prev_displ == displacement::right) ? displacement::right : displacement::left;
    }
    if(current_tile == 'F') {
        return (prev_displ == displacement::top) ? displacement::right : displacement::bottom;
    }
    if(current_tile == '7') {
        return (prev_displ == displacement::top) ? displacement::left : displacement::bottom;
    }
    if(current_tile == 'L') {
        return (prev_displ == displacement::bottom) ? displacement::right : displacement::top;
    }
    if(current_tile == 'J') {
        return (prev_displ == displacement::bottom) ? displacement::left : displacement::top;
    }
    throw unreachable();
}


uint32_t steps_to_farthest_point(const std::vector<std::string>& layout) {
    const coordinate start = get_start(layout);
    displacement displ = step_after_start(start, layout);
    coordinate current = start + displ;
    uint32_t distance  = 1;
    std::cout << start << " --(" << displ << ")--> " << current << std::endl;
    while(current != start) {
        std::cout << " '" << layout[current.y][current.x] << '\'';
        displ = next_displacement(current, layout, displ);
        current += displ;
        std::cout << " --(" << displ << ")--> " << current << std::endl;
        distance++;
    }
    std::cout << "Total loop length : " << distance << std::endl;
    return (distance+1)/2;
}


int main() {

    std::vector<std::string> layout;

    while(true) {
        std::optional<std::string> line = read_line();
        if(!line.has_value()) {
            break;
        }
        layout.push_back(std::move(line.value()));
    }

    const int32_t distance = steps_to_farthest_point(layout);
    std::cout << "Number of steps to get to the farthest point : " << distance << std::endl;

    return 0;
}

