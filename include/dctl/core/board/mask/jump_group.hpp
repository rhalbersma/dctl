#pragma once
#include <dctl/core/board/detail/coordinates.hpp>    // to_llo
#include <dctl/core/board/mask/detail/copy_if.hpp>   // copy_if
#include <dctl/util/type_traits.hpp>         // set_t, value_t
#include <xstd/cstdlib.hpp>                     // euclidean_div
#include <cassert>                              // assert

namespace dctl {
namespace core {
namespace board {
namespace mask {

template<class Board>
class jump_group
{
        template<int FromSquare>
        struct init
        {
                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()() const noexcept
                {
                        return detail::copy_if<Board>([](int const dest_sq){
                                auto const from_coord = board::detail::to_llo(FromSquare, Board::inner_grid);
                                auto const dest_coord = board::detail::to_llo(dest_sq   , Board::inner_grid);
                                auto const delta_x = xstd::euclidean_div(static_cast<int>(from_coord.x) - static_cast<int>(dest_coord.x), 4).rem;
                                auto const delta_y = xstd::euclidean_div(static_cast<int>(from_coord.y) - static_cast<int>(dest_coord.y), 4).rem;
                                return
                                        (delta_x == 0 && delta_y == 0) ||
                                        (delta_x == 2 && delta_y == 2)
                                ;
                        });
                }
        };

        static constexpr set_t<Board> value[] =
        {
                init<Board::edge_le() + 0>{}(),
                init<Board::edge_le() + 1>{}(),
                init<Board::edge_lo() + 0>{}(),
                init<Board::edge_lo() + 1>{}()
        };

public:
        constexpr auto operator()(int const n) const noexcept
        {
                assert(n < 4);
                return value[n];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace core
}       // namespace dctl
