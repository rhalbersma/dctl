#pragma once

namespace dctl {
namespace core {

enum class notation
{
        /* zero is reserved */
        algebraic = 1,
        numeric   = 2
};

namespace move {
namespace traits {

template<class Rules, class Board>
constexpr auto notation_v =
        Board::width == 10 ?
        notation::numeric :
        notation::algebraic
;

}       // namespace traits
}       // namespace move
}       // namespace core
}       // namespace dctl
