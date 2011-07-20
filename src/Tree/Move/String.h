#pragma once
#include <string>
#include "Protocol.h"
#include "Token.h"
#include "../Node/Move.h"

namespace tree {

namespace node { 
        template<typename> class Position; 
}
        
namespace move {
namespace string {

template
<
        typename Rules,
        typename Protocol = typename move::Protocol<Rules>::type,
        typename Token = typename move::Token<Rules>::type
>
struct read
{
	template<typename Board> 
        node::Move operator()(const node::Position<Board>&, const std::string&);
};

template
<
        typename Rules, 
        typename Protocol = typename move::Protocol<Rules>::type,
        typename Token = typename move::Token<Rules>::type
>
struct write
{
	template<typename Board> 
        std::string operator()(const node::Position<Board>&, const node::Move&);
};

}       // namespace string
}       // namespace move
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
