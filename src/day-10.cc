
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <queue>

#include <utils/split.hh>
#include <utils/input.hh>
#include <utils/unreachable.hh>


enum class pipe_joint {
    lr,
    bt,
    bl,
    br,
    tl,
    tr,
    none
};

pipe_joint joint_from_char(const char c) {
    switch(c) {
        case '-': return pipe_joint::lr;
        case '|': return pipe_joint::bt;
        case '7': return pipe_joint::bl;
        case 'F': return pipe_joint::br;
        case 'J': return pipe_joint::tl;
        case 'L': return pipe_joint::tr;
        default:  return pipe_joint::none;
    }
}


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


struct pipe_layout {
    std::vector<std::vector<pipe_joint>> tiles;
    coordinate start_position;
    displacement start_displacement;

    pipe_joint& operator[](const coordinate& c) {
        return tiles[c.y][c.x];
    }

    pipe_joint operator[](const coordinate& c) const {
        return tiles[c.y][c.x];
    }

};


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

std::ostream& operator<<(std::ostream& out, const pipe_joint p) {
    switch(p) {
        case pipe_joint::lr:   out << "─"; break;
        case pipe_joint::bt:   out << "│"; break;
        case pipe_joint::bl:   out << "┐"; break;
        case pipe_joint::br:   out << "┌"; break;
        case pipe_joint::tl:   out << "┘"; break;
        case pipe_joint::tr:   out << "└"; break;
        case pipe_joint::none: out << ' '; break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const pipe_layout& layout) {
    for(const std::vector<pipe_joint>& line: layout.tiles) {
        for(const pipe_joint joint: line) {
            out << joint;
        }
        out << std::endl;
    }
    return out;
}



coordinate get_start(const std::vector<std::string>& layout_repr) {
    coordinate res = {0, 0};
    for(const std::string& line: layout_repr) {
        const size_t pos = line.find('S');
        if(pos != std::string::npos) {
            res.x = pos;
            return res;
        }
        res.y++;
    }
    throw unreachable();
}

displacement step_after_start(const coordinate& start, const std::vector<std::string>& layout_repr) {
    if(start.x > 0) {
        const char left_tile = layout_repr[start.y][start.x-1];
        if(left_tile == '-' || left_tile == 'F' || left_tile == 'L') {
            return displacement::left;
        }
    }
    if(start.x < layout_repr[start.y].size() - 1) {
        const char right_tile = layout_repr[start.y][start.x+1];
        if(right_tile == '-' || right_tile == '7' || right_tile == 'J') {
            return displacement::right;
        }
    }
    if(start.y > 0) {
        const char top_tile = layout_repr[start.y-1][start.x];
        if(top_tile == '|' || top_tile == '7' || top_tile == 'F') {
            return displacement::top;
        }
    }
    if(start.y < layout_repr.size() - 1) {
        const char bottom_tile = layout_repr[start.y+1][start.x];
        if(bottom_tile == '|' || bottom_tile == 'J' || bottom_tile == 'L') {
            return displacement::bottom;
        }
    }
    throw unreachable();
}

displacement next_displacement(const coordinate& current_pos, const pipe_layout& layout, displacement prev_displ) {
    const pipe_joint current_tile = layout[current_pos];
    if(current_tile == pipe_joint::bt) {
        return (prev_displ == displacement::top) ? displacement::top : displacement::bottom;
    }
    if(current_tile == pipe_joint::lr) {
        return (prev_displ == displacement::right) ? displacement::right : displacement::left;
    }
    if(current_tile == pipe_joint::br) {
        return (prev_displ == displacement::top) ? displacement::right : displacement::bottom;
    }
    if(current_tile == pipe_joint::bl) {
        return (prev_displ == displacement::top) ? displacement::left : displacement::bottom;
    }
    if(current_tile == pipe_joint::tr) {
        return (prev_displ == displacement::bottom) ? displacement::right : displacement::top;
    }
    if(current_tile == pipe_joint::tl) {
        return (prev_displ == displacement::bottom) ? displacement::left : displacement::top;
    }
    throw unreachable();
}

pipe_layout make_layout(const std::vector<std::string>& layout_repr) {
    const coordinate start = get_start(layout_repr);
    displacement displ = step_after_start(start, layout_repr);
    pipe_layout layout = {
        std::vector<std::vector<pipe_joint>>(),
        start,
        displ
    };
    for(const std::string& line: layout_repr) {
        layout.tiles.push_back(std::vector<pipe_joint>(line.size(), pipe_joint::none));
    }
    coordinate current = start + displ;
    while(current != start) {
        layout[current] = joint_from_char(layout_repr[current.y][current.x]);
        displ = next_displacement(current, layout, displ);
        current += displ;
    }
    // Use the first and final displacements to compute the true joint at the start
    if((displ == displacement::bottom || displ == displacement::top) && (layout.start_displacement == displacement::bottom || layout.start_displacement == displacement::top)) {
        layout[start] = pipe_joint::bt;
    }
    if((displ == displacement::left || displ == displacement::right) && (layout.start_displacement == displacement::right || layout.start_displacement == displacement::left)) {
        layout[start] = pipe_joint::lr;
    }
    if((layout.start_displacement == displacement::bottom && displ == displacement::right) || (layout.start_displacement == displacement::left && displ == displacement::top)) {
        layout[start] = pipe_joint::bl;
    }
    if((layout.start_displacement == displacement::bottom && displ == displacement::left) || (layout.start_displacement == displacement::right && displ == displacement::top)) {
        layout[start] = pipe_joint::br;
    }
    if((layout.start_displacement == displacement::top && displ == displacement::right) || (layout.start_displacement == displacement::left && displ == displacement::bottom)) {
        layout[start] = pipe_joint::tl;
    }
    if((layout.start_displacement == displacement::top && displ == displacement::left) || (layout.start_displacement == displacement::right && displ == displacement::bottom)) {
        layout[start] = pipe_joint::tr;
    }
    return layout;
}


uint32_t steps_to_farthest_point(const pipe_layout& layout) {
    uint32_t distance  = 1;
    coordinate current = layout.start_position + layout.start_displacement;
    displacement displ = layout.start_displacement;
    while(current != layout.start_position) {
        displ = next_displacement(current, layout, displ);
        current += displ;
        distance++;
    }
    std::cout << "Total loop length : " << distance << std::endl;
    return (distance+1)/2;
}


void propagate(bool& b1, bool& b2) {
    b1 = b1 || b2;
    b2 = b1;
}

void propagate(bool& b1, bool& b2, bool& b3) {
    b1 = b1 || b2 || b3;
    b2 = b1;
    b3 = b1;
}

void propagate(bool& b1, bool& b2, bool& b3, bool& b4) {
    b1 = b1 || b2 || b3 || b4;
    b2 = b1;
    b3 = b1;
    b4 = b1;
}

struct flood_tile {
    flood_tile(size_t x, size_t y, pipe_joint joint): pos({x, y}), joint(joint) {}

    bool tr = false;
    bool tl = false;
    bool br = false;
    bool bl = false;
    bool pushed = false;
    const coordinate pos;
    const pipe_joint joint;

    bool partially_flooded() const { return tr || tl || br || bl; }
    bool flooded() const { return tr && tl && br && bl; }
    bool inside() const { return (joint == pipe_joint::none) && !flooded(); }

    /// @brief propagate the values inside the tile
    void self_propagate() {
        switch(joint) {
            case pipe_joint::none: propagate(tl, tr, bl, br);            return;
            case pipe_joint::lr:   propagate(tl, tr); propagate(bl, br); return;
            case pipe_joint::bt:   propagate(tl, bl); propagate(tr, br); return;
            case pipe_joint::tl:   propagate(tr, br, bl);                return;
            case pipe_joint::tr:   propagate(tl, bl, br);                return;
            case pipe_joint::bl:   propagate(tl, tr, br);                return;
            case pipe_joint::br:   propagate(tr, tl, bl);                return;
            default: return;
        }
    }
};



void flood(std::shared_ptr<flood_tile> tile, const std::vector<std::vector<std::shared_ptr<flood_tile>>>& flood_compute, std::queue<std::shared_ptr<flood_tile>>& flood_queue) {
    tile->self_propagate();
    // Propagate with adjacent left tile
    if(tile->pos.x > 0) {
        std::shared_ptr<flood_tile> left_tile = flood_compute[tile->pos.y][tile->pos.x-1];
        if(tile->joint == pipe_joint::none || tile->joint == pipe_joint::bt || tile->joint == pipe_joint::tr || tile->joint == pipe_joint::br) {
            propagate(tile->tl, tile->bl, left_tile->tr, left_tile->br);
        } else {
            propagate(tile->tl, left_tile->tr);
            propagate(tile->bl, left_tile->br);
        }
        if(!left_tile->pushed && tile->partially_flooded()) {
            flood_queue.push(left_tile);
            left_tile->pushed = true;
        }
    }
    // Propagate with adjacent right tile
    if(tile->pos.x < flood_compute[tile->pos.y].size() - 1) {
        std::shared_ptr<flood_tile> right_tile = flood_compute[tile->pos.y][tile->pos.x+1];
        if(tile->joint == pipe_joint::none || tile->joint == pipe_joint::bt || tile->joint == pipe_joint::tl || tile->joint == pipe_joint::bl) {
            propagate(tile->tr, tile->br, right_tile->tl, right_tile->bl);
        } else {
            propagate(tile->tr, right_tile->tl);
            propagate(tile->br, right_tile->bl);
        }
        if(!right_tile->pushed && tile->partially_flooded()) {
            flood_queue.push(right_tile);
            right_tile->pushed = true;
        }
    }
    // Propagate with adjacent top tile
    if(tile->pos.y > 0) {
        std::shared_ptr<flood_tile> top_tile = flood_compute[tile->pos.y-1][tile->pos.x];
        if(tile->joint == pipe_joint::none || tile->joint == pipe_joint::lr || tile->joint == pipe_joint::br || tile->joint == pipe_joint::bl) {
            propagate(tile->tl, tile->tr, top_tile->bl, top_tile->br);
        } else {
            propagate(tile->tl, top_tile->bl);
            propagate(tile->tr, top_tile->br);
        }
        if(!top_tile->pushed && tile->partially_flooded()) {
            flood_queue.push(top_tile);
            top_tile->pushed = true;
        }
    }
    // Propagate with adjacent bottom tile
    if(tile->pos.y < flood_compute.size() - 1) {
        std::shared_ptr<flood_tile> bottom_tile = flood_compute[tile->pos.y+1][tile->pos.x];
        if(tile->joint == pipe_joint::none || tile->joint == pipe_joint::lr || tile->joint == pipe_joint::tr || tile->joint == pipe_joint::tl) {
            propagate(tile->bl, tile->br, bottom_tile->tl, bottom_tile->tr);
        } else {
            propagate(tile->bl, bottom_tile->tl);
            propagate(tile->br, bottom_tile->tr);
        }
        if(!bottom_tile->pushed && tile->partially_flooded()) {
            flood_queue.push(bottom_tile);
            bottom_tile->pushed = true;
        }
    }
    tile->self_propagate();
}

uint32_t compute_enclosed_tiles(const pipe_layout& layout) {
    size_t y = 0;
    std::vector<std::vector<std::shared_ptr<flood_tile>>> flood_compute;
    for(const std::vector<pipe_joint>& line: layout.tiles) {
        flood_compute.emplace_back();
        for(size_t x = 0; x < line.size(); x++) {
            flood_compute.back().push_back(std::make_shared<flood_tile>(x, y, line[x]));
        }
        y++;
    }

    flood_compute[0][0]->tl = true; // We start flooding from the top left corner of the map
    std::queue<std::shared_ptr<flood_tile>> flood_queue;
    flood_queue.push(flood_compute[0][0]);
    flood_compute[0][0]->pushed = true;

    while(flood_queue.size() > 0) {
        std::shared_ptr<flood_tile> current = flood_queue.front();
        flood_queue.pop();
        flood(current, flood_compute, flood_queue);
    }

    for(const std::vector<std::shared_ptr<flood_tile>>& flood_line: flood_compute) {
        for(const std::shared_ptr<flood_tile>& tile: flood_line) {
            std::cout << (tile->tl ? '~' : '#') << ' ' << (tile->tr ? '~' : '#') << ' ';
        }
        std::cout << std::endl;
        for(const std::shared_ptr<flood_tile>& tile: flood_line) {
            std::cout << (tile->bl ? '~' : '#') << ' ' << (tile->br ? '~' : '#') << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    uint32_t res = 0;
    for(const std::vector<std::shared_ptr<flood_tile>>& flood_line: flood_compute) {
        for(const std::shared_ptr<flood_tile>& tile: flood_line) {
            if(tile->inside()) {
                res++;
                std::cout << 'O';
            } else {
                std::cout << tile->joint;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return res;
}


int main(int argc, char** argv) {

    const bool is_part_2 = (argc >= 2 && std::string("--part2") == argv[1]);

    std::vector<std::string> layout_repr;

    while(true) {
        std::optional<std::string> line = read_line();
        if(!line.has_value()) {
            break;
        }
        layout_repr.push_back(std::move(line.value()));
    }

    const pipe_layout layout = make_layout(layout_repr);

    std::cout << layout << std::endl;

    if(is_part_2) {
        const int32_t nb_tiles = compute_enclosed_tiles(layout);
        std::cout << "Number of enclosed tiles : " << nb_tiles << std::endl;
    } else {
        const int32_t distance = steps_to_farthest_point(layout);
        std::cout << "Number of steps to get to the farthest point : " << distance << std::endl;
    }

    return 0;
}


