#pragma once
#include <string>
#include "../setup/Setup.h"

namespace dctl { 
        
template<typename> class Position;
class Material;

template
<
        typename Protocol,
        typename Token = setup::Setup<Protocol>
>
struct Diagram
{
	template<typename Board> 
        const std::string operator()(const Position<Board>&) const;
};

template<typename> 
std::string bit_content(const Material&, size_t);

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Diagram.hpp"
