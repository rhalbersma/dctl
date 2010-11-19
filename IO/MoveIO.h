#pragma once
#include <string>

template<typename> class Position;
class Pieces;

template<typename>
struct write_move_string
{
	template<typename Board> std::string operator()(const Position<Board>&, const Pieces&);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "MoveIO.hpp"
