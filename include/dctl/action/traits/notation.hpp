#pragma once
#include <type_traits>  // conditional, integral_constant

namespace dctl {

enum class Notation
{
        /* zero is reserved */
        algebraic = 1,
        numeric   = 2
};

namespace move {
namespace traits {

template<Notation N>
using notation_constant = std::integral_constant<Notation, N>;

template<class Rules, class Board>
struct xnotation
:
        std::conditional_t<
                (Board::width() == 10),
                notation_constant<Notation::numeric>,
                notation_constant<Notation::algebraic>
        >
{};

}       // namespace traits
}       // namespace move
}       // namespace dctl
