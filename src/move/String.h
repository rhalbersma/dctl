#pragma once
#include <string>
#include "Protocol.h"
#include "Token.h"
#include "../node/Move.h"

namespace dctl {

template<typename> class Position;
        
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
        Move operator()(const Position<Board>&, const std::string&);
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
        std::string operator()(const Position<Board>&, const Move&);
};

}       // namespace string
}       // namespace move
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
