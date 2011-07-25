#pragma once
#include <functional>
#include <string>

namespace dctl {
namespace board {

template
<
        typename Board, 
        typename Content
>
struct Diagram
{
        const std::string operator()() const;
        
        template<typename F> 
        const std::string operator()(F) const;
};

template<typename> bool is_end_row(int);
template<typename> bool is_indent_row(int);

}       // namespace board
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Diagram.hpp"
