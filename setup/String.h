#pragma once
#include <string>
#include "Token.h"

namespace dctl {

template<typename> class Position;

namespace setup {

template
<
        typename Board,
        typename Protocol, 
        typename Content = typename Token<Protocol>::type
>
struct read
{
	Position<Board> operator()(const std::string&) const;
};

template
<
        typename Protocol, 
        typename Content = typename Token<Protocol>::type
>
struct write
{
	template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

}       // namespace setup
}       // namespace dctl

// include template definitions inside header
#include "String.hpp"
