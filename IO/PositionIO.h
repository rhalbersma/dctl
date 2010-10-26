#pragma once
#include "Token.h"
#include "../Board/Board.h"
#include <functional>
#include <iostream>
#include <string>

template<typename> class Position;

struct ForsythEdwardsNotation {};
struct DamExchangeProtocol {};

typedef ForsythEdwardsNotation FEN;
typedef DamExchangeProtocol DXP;

template<typename> struct DefaultToken;

template<>
struct DefaultToken<FEN>
{
	typedef FEN_PositionToken TYPE;
};

template<>
struct DefaultToken<DXP>
{
	typedef DXP_PositionToken TYPE;
};

template
<
        typename Protocol, 
        typename Board = DefaultBoard, 
        typename Token = typename DefaultToken<Protocol>::TYPE
>
struct read_position_string
{
	Position<Board> operator()(const std::string&);
};

template<typename Board, typename Token>
struct read_position_string<FEN, Board, Token>
{
	Position<Board> operator()(const std::string&);
};

template<typename Board, typename Token>
struct read_position_string<DXP, Board, Token>
{
	Position<Board> operator()(const std::string&);
};

template
<
        typename Protocol, 
        typename Token = typename DefaultToken<Protocol>::TYPE
>
struct write_position_string
{
	template<typename Board> std::string operator()(const Position<Board>&) const;
};

template<typename Token>
struct write_position_string<FEN, Token>
{
	template<typename Board> std::string operator()(const Position<Board>&) const;
};

template<typename Token>
struct write_position_string<DXP, Token>
{
	template<typename Board> std::string operator()(const Position<Board>&) const;
};

template
<
        typename Protocol,
        typename Token = typename DefaultToken<Protocol>::TYPE
>
struct write_position_layout
{
	template<typename Board> std::string operator()(const Position<Board>&) const;
};

template<typename Board, typename Token>
struct write_position_bit: public std::binary_function<Position<Board>, size_t, std::string>
{
        std::string operator()(const Position<Board>&, size_t) const;
};

template<typename> 
bool is_valid_square(size_t);

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "PositionIO.hpp"
