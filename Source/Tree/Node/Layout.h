#pragma once
#include <string>

namespace Tree { 
namespace Node { 
        
template<typename> class Position;

namespace Layout {

template
<
        typename Protocol,
        typename Token = Node::Token<Protocol>
>
struct write
{
	template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

template<typename> 
std::string bit_content(const Pieces&, size_t);

}       // namespace Layout
}       // namespace Node
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Layout.hpp"
