#pragma once
#include <dctl/angle/detail/abs_remainder.hpp>
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/grid/coordinates.hpp>
#include <array>                                // array

namespace dctl {
namespace board {

template<class Board>
class JumpGroup
{
private:
        // simulate a constexpr lambda (not allowed in C++14)
        struct lambda
        {
                int from_bit_;

                constexpr auto operator()(int dest_bit) const noexcept
                {
                        auto const from_sq = grid::ulo::Square<typename Board::external_grid>{Board::square_from_bit(from_bit_)};
                        auto const dest_sq = grid::ulo::Square<typename Board::external_grid>{                       dest_bit  };
                        auto const from_coord = ulo_from_sq(from_sq);
                        auto const dest_coord = ulo_from_sq(dest_sq);
                        auto const delta_x = dctl::detail::abs_remainder(get_x(from_coord) - get_x(dest_coord), 4);
                        auto const delta_y = dctl::detail::abs_remainder(get_y(from_coord) - get_y(dest_coord), 4);
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
        using Set = typename Board::set_type;
        using table_type = std::array<Set, N>;
        static constexpr auto GG = Board::inner_grid;

        static constexpr table_type table =
        {{
                init(0),
                init(1),
                init(GG.left_down() + (ul_parity(GG) ? 2 : 0)),
                init(GG.right_down())
        }};

public:
        static constexpr auto mask(int n) noexcept
        {
                return table[static_cast<std::size_t>(n)];
        }
};

template<class Board>
constexpr typename JumpGroup<Board>::table_type
JumpGroup<Board>::table;

}       // namespace board
}       // namespace dctl
