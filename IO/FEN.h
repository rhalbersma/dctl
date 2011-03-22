#pragma once
#include "PositionIO.h"
#include "Token.h"

struct FEN_tag {};

template<>
struct PositionToken<FEN_tag>: public PositionTokenTriple<'B', 'W', '.'>
{
        static const char KING  = 'K';
        static const char COLON = ':';
        static const char COMMA = ',';
        static const char QUOTE = '\"';
};

template<typename Board, typename Token>
struct read_position_string<Board, FEN_tag, Token>
{
	Tree::Node::Position<Board> operator()(const std::string&);
};

template<typename Token>
struct write_position_string<FEN_tag, Token>
{
	template<typename Board> 
        std::string operator()(const Tree::Node::Position<Board>&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "FEN.hpp"
