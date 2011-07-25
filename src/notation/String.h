#pragma once
#include <string>
#include "Notation.h"

namespace dctl {

template<typename> class Position;
class Move;
        
namespace notation {

template
<
        typename Rules,
        typename Format = typename Format<Rules>::type,
        typename Separator = typename Separator<Rules>::type
>
struct read
{
	template<typename Board> 
        const Move operator()(const Position<Board>&, const std::string&) const;
};

template
<
        typename Rules, 
        typename Format = typename Format<Rules>::type,
        typename Separator = typename Separator<Rules>::type
>
struct write
{
	template<typename Board> 
        const std::string operator()(const Position<Board>&, const Move&) const;
};

}       // namespace notation
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
