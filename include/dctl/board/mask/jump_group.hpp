#pragma once
#include <dctl/angle/detail/abs_remainder.hpp>  // abs_remainder
#include <dctl/board/coordinates.hpp>           // to_ulo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/set_type.hpp>                    // set_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class JumpGroup
{
        // simulate a constexpr lambda (not allowed in C++14)
        struct lambda
        {
                int from_sq;

                constexpr auto operator()(int dest_sq) const noexcept
                {
                        auto const from_coord = to_ulo(from_sq, Board::outer_grid);
                        auto const dest_coord = to_ulo(dest_sq, Board::outer_grid);
                        auto const delta_x = dctl::detail::abs_remainder(from_coord.x - dest_coord.x, 4);
                        auto const delta_y = dctl::detail::abs_remainder(from_coord.y - dest_coord.y, 4);
                        return
                                (delta_x == 0 && delta_y == 0) ||
                                (delta_x == 2 && delta_y == 2)
                        ;
                }
        };

        static constexpr auto init(int n) noexcept
        {
                return make_set_if<Board>(lambda{n});
        }

        static constexpr auto N = 4;
        using Set = set_type<Board>;
        using table_type = std::array<Set, N>;

        static constexpr table_type table =
        {{
                init(Board::edge_le() + 0),
                init(Board::edge_le() + 1),
                init(Board::edge_lo() + 0),
                init(Board::edge_lo() + 1)
        }};

public:
        static constexpr auto mask(std::size_t n) noexcept
        {
                assert(n < N);
                return table[n];
        }
};

template<class Board>
constexpr typename JumpGroup<Board>::table_type
JumpGroup<Board>::table;

}       // namespace board
}       // namespace dctl
