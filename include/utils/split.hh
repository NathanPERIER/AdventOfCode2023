
#pragma once

#include <string>


class split {

public:
    split(const std::string& view, const std::string& sep): _view(view), _separator(sep) {}

    class iterator {

    public:
        static iterator make_begin(std::string_view view, std::string_view sep) { return iterator(view, sep, 0);           }
        static iterator make_end(std::string_view view, std::string_view sep)   { return iterator(view, sep, view.size()); }

        iterator& operator++() { 
            _begin = _end + _sep.size();
            if(_begin > _view.size()) {
                _begin = _view.size();
            }
            _end = next_pos(_view, _sep, _begin);
            return *this;
        }
        iterator operator++(int) { iterator res = *this; ++(*this); return res; }

        bool operator==(const iterator& it) const { 
            return _view.data() == it._view.data() 
                && _sep.data()  == it._sep.data()
                && _begin       == it._begin
                && _end         == it._end;
        }
        bool operator!=(const iterator& it) const { return !(*this == it); }

        std::string_view operator*() { return _view.substr(_begin, _end - _begin); }

        // iterator traits
        using difference_type = std::ptrdiff_t;
        using value_type = std::string_view;
        using pointer = const std::string_view*;
        using reference = const std::string_view&;
        using iterator_category = std::forward_iterator_tag;

    protected:
        // End
        iterator(std::string_view view, std::string_view sep): _view(view), _sep(sep), _begin(view.size()), _end(view.size()) {}
        iterator(std::string_view view, std::string_view sep, std::size_t pos): _view(view), _sep(sep), _begin(pos), _end(next_pos(_view, _sep, _begin)) {}

    private:
        const std::string_view _view;
        const std::string_view _sep;
        std::size_t _begin;
        std::size_t _end;

        static std::size_t next_pos(std::string_view view, std::string_view sep, std::size_t pos) {
            if(pos >= view.size()) {
                return view.size();
            }
            const std::size_t res = view.find(sep, pos);
            return (res == std::string_view::npos) ? view.size() : res;
        }

    };

    iterator begin() { return iterator::make_begin(_view, _separator); }
    iterator end()   { return iterator::make_end(_view, _separator);   }

private:
    std::string_view _view;
    std::string _separator;

};

