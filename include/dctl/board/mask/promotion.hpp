#pragma once
#include <dctl/board/mask/row.hpp>      // Row
#include <dctl/color.hpp>               // Player

namespace dctl {
namespace board {

template<class Board>
struct Promotion
{
        static constexpr auto mask(color const c) noexcept
        {
                return Row<Board>::mask(!c, 0);
        }
};

}       // namespace board
}       // namespace dctl
