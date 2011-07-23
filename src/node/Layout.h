#pragma once
#include <string>

namespace dctl { 
        
template<typename> class Position;

namespace layout {

template
<
        typename Protocol,
        typename Token = Token<Protocol>
>
struct write
{
	template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

template<typename> 
std::string bit_content(const Material&, size_t);

}       // namespace layout
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Layout.hpp"
