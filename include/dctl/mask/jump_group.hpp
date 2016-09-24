#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/mask/detail/copy_if.hpp>         // copy_if
#include <dctl/utility/type_traits.hpp>         // set_t, value_t
#include <xstd/cstdlib.hpp>                     // euclidean_div
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace mask {

template<class Board>
class jump_group
{
        template<std::size_t FromSquare>
        struct init
        {
                struct is_jump_group
                {
                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(std::size_t const dest_sq) const noexcept
                        {
                                auto const from_coord = board::detail::to_llo(FromSquare, Board::inner_grid);
                                auto const dest_coord = board::detail::to_llo(dest_sq   , Board::inner_grid);
                                auto const delta_x = xstd::euclidean_div(static_cast<int>(from_coord.x) - static_cast<int>(dest_coord.x), 4).rem;
                                auto const delta_y = xstd::euclidean_div(static_cast<int>(from_coord.y) - static_cast<int>(dest_coord.y), 4).rem;
                                return
                                        (delta_x == 0 && delta_y == 0) ||
                                        (delta_x == 2 && delta_y == 2)
                                ;
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()() const noexcept
                {
                        return detail::copy_if<Board>(is_jump_group{});
                }
        };

        using value_type = set_t<Board>;

        static constexpr value_type value[] =
        {
                init<Board::edge_le() + 0>{}(),
                init<Board::edge_le() + 1>{}(),
                init<Board::edge_lo() + 0>{}(),
                init<Board::edge_lo() + 1>{}()
        };

public:
        constexpr auto operator()(std::size_t const n) const noexcept
        {
                assert(n < 4);
                return value[n];
        }
};

template<class Board>
constexpr value_t<jump_group<Board>>
jump_group<Board>::value[];

}       // namespace mask
}       // namespace dctl
