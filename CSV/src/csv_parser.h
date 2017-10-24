#ifndef CSV_CSV_PARSER_H
#define CSV_CSV_PARSER_H


#include "tuple.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>

template< typename T >
bool read_tuple_element(std::istream& in, T& value) {
    in >> value;
    return true;
}

template< typename Tuple, std::size_t... I >
void read_tuple_elements(std::istream& in, Tuple& value, std::index_sequence<I...>) {
    std::initializer_list<bool>{read_tuple_element(in, std::get<I>(value))...};
}

template< typename ... T >
class CsvParser {
public:
    class Iter {
    public:
        Iter(CsvParser<T...> *p_parser) : _p_parser(p_parser), _first(true) {

        }

        bool
        operator!=(const Iter& other) const {
            return !_p_parser->_is.eof() || !other._p_parser->_is.eof();
        }

        std::tuple<T...> operator*() {
            if (_first) {
                ++*this;
            }
            return _p_parser->_tuple;
        }

        const Iter& operator++() {
            _first = false;
            std::tuple<T...> t;
            read_tuple_elements(_p_parser->_is, t, std::make_index_sequence<sizeof...(T)>{ });
            _p_parser->_tuple = std::move(t);

            return *this;
        }
    private:
        CsvParser<T...> *_p_parser;
        bool _first;
    };

    CsvParser(std::string const& filename) : _is(filename) {
        if (!_is.is_open()) {
            throw std::invalid_argument("File not found");
        }
    };

    std::tuple<T...> _tuple;
    std::ifstream _is;

    Iter begin() {
        return Iter(this);
    }

    Iter end() {
        return Iter(this);
    }
};


#endif //CSV_CSV_PARSER_H
