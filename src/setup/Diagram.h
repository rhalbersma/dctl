#pragma once
#include <string>       // std::string
#include "Setup.h"

namespace dctl {

template<typename> class Position;

namespace setup {

template
<
        typename Protocol, 
        typename Setup = Setup<Protocol>
>
class diagram
{
public:
        // the position square content
        template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

}       // namespace setup
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Diagram.hpp"
