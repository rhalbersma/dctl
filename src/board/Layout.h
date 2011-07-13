#pragma once
#include <functional>
#include <string>

namespace board {

template
<
        typename Board, 
        typename Protocol
>
struct write: public std::unary_function<void, std::string>
{
        std::string operator()() const;
        
        template<typename F> 
        std::string operator()(F) const;
};

template<typename> bool is_end_row(int);
template<typename> bool is_indent_row(int);

}       // namespace board

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Layout.hpp"
