#pragma once
#include <dctl/rule_traits.hpp>
#include <type_traits>          // integral_constant

namespace dctl {

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
