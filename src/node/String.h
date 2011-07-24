#pragma once
#include <string>
#include "Position.h"
#include "../protocol/setup/Setup.h"

namespace dctl {
namespace string {

template
<
        typename Board,
        typename Protocol, 
        typename Token = protocol::setup::Setup<Protocol>
>
struct read
{
	Position<Board> operator()(const std::string&);
};

template
<
        typename Protocol, 
        typename Token = protocol::setup::Setup<Protocol>
>
struct write
{
	template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

}       // namespace string
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
