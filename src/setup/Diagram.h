#pragma once
#include <string>
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
        const std::string operator()() const;
        
        template<typename F> 
        const std::string operator()(F) const;

        template<typename Board> 
        const std::string operator()(const Position<Board>&) const;
};

template<typename> bool is_end_row(int);
template<typename> bool is_indent_row(int);
template<typename> std::string bit_content(const Material&, size_t);

}       // namespace setup
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Diagram.hpp"
