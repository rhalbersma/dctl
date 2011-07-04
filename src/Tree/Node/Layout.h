#pragma once
#include <string>

namespace tree { 
namespace node { 
        
template<typename> class Position;

namespace layout {

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

template<typename> 
std::string bit_content(const Pieces&, size_t);

}       // namespace layout
}       // namespace node
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Layout.hpp"
