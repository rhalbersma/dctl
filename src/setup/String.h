#pragma once
#include <string>
#include "Setup.h"

namespace dctl {

template<typename> class Position;

namespace setup {

template
<
        typename Board,
        typename Protocol, 
        typename Token = Setup<Protocol>
>
struct read
{
	const Position<Board> operator()(const std::string&) const;
};

template
<
        typename Protocol, 
        typename Token = Setup<Protocol>
>
struct write
{
	template<typename Board> 
        const std::string operator()(const Position<Board>&) const;
};

}       // namespace setup
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
