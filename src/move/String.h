#pragma once
#include <string>
#include "../node/Move.h"
#include "../protocol/notation/Notation.h"

namespace dctl {

template<typename> class Position;
        
namespace notation {
namespace string {

template
<
        typename Rules,
        typename Format = typename protocol::notation::Format<Rules>::type,
        typename Separator = typename protocol::notation::Separator<Rules>::type
>
struct read
{
	template<typename Board> 
        Move operator()(const Position<Board>&, const std::string&);
};

template
<
        typename Rules, 
        typename Format = typename protocol::notation::Format<Rules>::type,
        typename Separator = typename protocol::notation::Separator<Rules>::type
>
struct write
{
	template<typename Board> 
        std::string operator()(const Position<Board>&, const Move&);
};

}       // namespace string
}       // namespace notation
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "String.hpp"
