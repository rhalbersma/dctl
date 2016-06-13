#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/cstdlib.hpp>                     // euclidean_div
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class JumpGroup
{
        static constexpr auto init(std::size_t const from_sq) noexcept
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_jump_group
                {
                        std::size_t const from_sq_;

                        constexpr auto operator()(std::size_t const dest_sq) const noexcept
                        {
                                auto const from_coord = detail::to_llo(from_sq_, Board::inner_grid);
                                auto const dest_coord = detail::to_llo(dest_sq , Board::inner_grid);
                                auto const delta_x = xstd::euclidean_div(static_cast<int>(from_coord.x) - static_cast<int>(dest_coord.x), 4).rem;
                                auto const delta_y = xstd::euclidean_div(static_cast<int>(from_coord.y) - static_cast<int>(dest_coord.y), 4).rem;
                                return
                                        (delta_x == 0 && delta_y == 0) ||
                                        (delta_x == 2 && delta_y == 2)
                                ;
                        }
                };

                return make_set_if<Board>(is_jump_group{from_sq});
        }

        using table_type = std::array<set_t<Board>, 4>;
        static constexpr table_type table =
        {{
                init(Board::edge_le() + 0),
                init(Board::edge_le() + 1),
                init(Board::edge_lo() + 0),
                init(Board::edge_lo() + 1)
        }};

public:
        static constexpr auto mask(std::size_t const n) noexcept
        {
                assert(n < 4);
                return table[n];
        }
};

template<class Board>
constexpr typename JumpGroup<Board>::table_type
JumpGroup<Board>::table;

}       // namespace board
}       // namespace dctl
