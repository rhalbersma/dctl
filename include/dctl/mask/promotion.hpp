#pragma once
#include <dctl/color.hpp>       // color
#include <dctl/mask/row.hpp>    // row

namespace dctl {
namespace mask {

template<class Board>
struct promotion
{
        constexpr auto operator()(color const to_move) const noexcept
        {
                return row<Board>{}(!to_move, 0);
        }
};

template<class Board, color ToMove>
constexpr auto promotion_v = promotion<Board>{}(ToMove);

}       // namespace mask
}       // namespace dctl
