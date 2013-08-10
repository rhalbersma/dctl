#pragma once

namespace dctl {
namespace notation {
namespace detail {

template<char M, char J>
struct token
{
		// BOOST_STATIC_CONSTANT here will give a linker error
		// when called without optimization flags g++ -O0
		enum { move = M, jump = J };
};

}

struct numeric {};              // numeric
using algebraic = numeric;      // algebraic

}       // namespace notation
}       // namespace dctl
