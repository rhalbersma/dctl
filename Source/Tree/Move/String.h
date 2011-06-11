#pragma once
#include <string>
#include "Protocol.h"
#include "Token.h"

namespace tree {

namespace node { 
        template<typename> class Position; 
        class Pieces;
}
        
namespace move {
namespace string {

template
<
        typename Rules,
        typename Protocol = typename move::Protocol<Rules>::TYPE,
        typename Token = typename move::Token<Rules>::TYPE
>
struct read
{
	template<typename Board> 
        node::Pieces operator()(const node::Position<Board>&, const std::string&);
};

template
<
        typename Rules, 
        typename Protocol = typename move::Protocol<Rules>::TYPE,
        typename Token = typename move::Token<Rules>::TYPE
>
struct write
{
	template<typename Board> 
        std::string operator()(const node::Position<Board>&, const node::Pieces&);
};

}       // namespace string
}       // namespace move
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
