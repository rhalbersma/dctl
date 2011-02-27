#pragma once
#include <functional>
#include <string>

template<typename> class Position;
template<typename> struct PositionToken;

template
<
        typename Protocol, 
        typename Board,
        typename Token = PositionToken<Protocol>
>
struct read_position_string
{
	Position<Board> operator()(const std::string&);
};

template
<
        typename Protocol, 
        typename Token = PositionToken<Protocol>
>
struct write_position_string
{
	template<typename Board> std::string operator()(const Position<Board>&) const;
};

template
<
        typename Protocol,
        typename Token = PositionToken<Protocol>
>
struct write_position_layout
{
	template<typename Board> std::string operator()(const Position<Board>&) const;
};

template
<
        typename Board, 
        typename Token
>
struct write_position_bit: public std::binary_function<Position<Board>, size_t, std::string>
{
        std::string operator()(const Position<Board>&, size_t) const;
};

template<typename> 
bool is_valid_square(size_t);

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PositionIO.hpp"

