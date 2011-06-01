#pragma once
#include <string>
#include "Protocol.h"
#include "Token.h"

namespace Tree {

namespace Node { 
        template<typename> class Position; 
        class Pieces;
}
        
namespace Move {
namespace String {

template
<
        typename Rules,
        typename Protocol = typename Move::Protocol<Rules>::TYPE,
        typename Token = typename Move::Token<Rules>::TYPE
>
struct read
{
	template<typename Board> 
        Node::Pieces operator()(const Node::Position<Board>&, const std::string&);
};

template
<
        typename Rules, 
        typename Protocol = typename Move::Protocol<Rules>::TYPE,
        typename Token = typename Move::Token<Rules>::TYPE
>
struct write
{
	template<typename Board> 
        std::string operator()(const Node::Position<Board>&, const Node::Pieces&);
};

}       // namespace String
}       // namespace Move
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
