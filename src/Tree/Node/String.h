#pragma once
#include <string>
#include "Protocol.h"
#include "Position.h"
#include "Token.h"

namespace tree {
namespace node {
namespace string {

template
<
        typename Board,
        typename Protocol, 
        typename Token = node::Token<Protocol>
>
struct read
{
	Position<Board> operator()(const std::string&);
};

template
<
        typename Protocol, 
        typename Token = node::Token<Protocol>
>
struct write
{
	template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

}       // namespace string
}       // namespace node
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
