#pragma once
#include "Token.h"
#include "../IO/PositionIO.h"
#include <string>

template<typename Board, typename Token>
struct read_position_string<Board, DXP_tag, Token>
{
	Position<Board> operator()(const std::string&);
};

template<typename Token>
struct write_position_string<DXP_tag, Token>
{
	template<typename Board> std::string operator()(const Node::Position<Board>&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "PositionIO.hpp"
