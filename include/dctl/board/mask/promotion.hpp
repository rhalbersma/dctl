#pragma once
#include <dctl/board/mask/row.hpp>

namespace dctl {
namespace board {

template<class Board>
struct Promotion
{
        static constexpr auto mask(bool color) noexcept
        {
                return Row<Board>::mask(color, 0);
        }
};

}       // namespace board
}       // namespace dctl
