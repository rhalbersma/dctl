#pragma once
#include <string>       // std::string
#include "Setup.h"

namespace dctl {

template<typename> class Position;
class Material;

namespace setup {

template
<
        typename Protocol, 
        typename Setup = Setup<Protocol>
>
struct diagram
{
        std::string operator()() const;
        
        template<typename F> 
        std::string operator()(F) const;

        template<typename Board> 
        std::string operator()(const Position<Board>&) const;
};

template<typename> bool is_end_row(int);
template<typename> bool is_indent_row(int);
template<typename> std::string bit_content(const Material&, int);

}       // namespace setup
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Diagram.hpp"
