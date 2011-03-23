#pragma once
#include <functional>
#include <string>

namespace Geometry {
namespace Layout {

template
<
        typename Board, 
        typename Protocol
>
struct write: public std::unary_function<void, std::string>
{
public:
        std::string operator()(void) const;
        
        template<typename F> 
        std::string operator()(F) const;
};

template<typename> bool bit2square(size_t);
template<typename> bool is_valid(size_t);
template<typename> bool is_end_row(size_t);
template<typename> bool is_indent_row(size_t);

}       // namespace Layout
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Layout.hpp"
