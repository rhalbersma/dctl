#pragma once
#include <dctl/board/mask/row.hpp>

namespace dctl {
namespace board {

template<class Board>
struct Promotion
{
        static constexpr auto mask(Color c) noexcept
        {
                return Row<Board>::mask(!c, 0);
        }
};

}       // namespace board
}       // namespace dctl