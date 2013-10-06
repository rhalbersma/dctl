#pragma once

namespace dctl {
namespace notation {
namespace detail {

template<char M, char J>
struct token
{
        enum { move = M, jump = J };
};

}

struct numeric {};              // numeric
using algebraic = numeric;      // algebraic

}       // namespace notation
}       // namespace dctl
