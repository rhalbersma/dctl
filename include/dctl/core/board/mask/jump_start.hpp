#pragma once
#include <dctl/core/board/angle.hpp>                 // angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/core/board/detail/coordinates.hpp>    // to_ulo
#include <dctl/core/board/mask/detail/copy_if.hpp>   // copy_if
#include <dctl/util/fill_array.hpp>          // fill_array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {
namespace board {
namespace mask {

template<class Board>
class jump_start
{
        static constexpr auto theta = Board::is_orthogonal_jump ? 45_deg : 90_deg;
        static constexpr auto beta  = Board::is_orthogonal_jump ?  0_deg : 45_deg;
        static constexpr auto N      = Board::is_orthogonal_jump ?      8 :      4;
        static constexpr auto value = fill_array<N>([](int const segment){
                return detail::copy_if<Board>([=](auto const sq){
                        auto const alpha = rotate(segment * theta + beta, inverse(Board::orientation));
                        auto const offset = is_diagonal(alpha) ? 2 : 4;
                        auto const min_x = is_left(alpha) ? offset : 0;
                        auto const max_x = Board::width - (is_right(alpha) ? offset : 0);
                        auto const min_y = is_up(alpha) ? offset : 0;
                        auto const max_y = Board::height - (is_down(alpha) ? offset : 0);
                        auto const coord = board::detail::to_ulo(sq, Board::inner_grid);
                        return
                                (min_x <= coord.x && coord.x < max_x) &&
                                (min_y <= coord.y && coord.y < max_y)
                        ;
                });
        });
public:
        constexpr auto operator()(angle const alpha) const noexcept
        {
                auto const segment = (alpha - beta) / theta;
                assert(segment < N);
                return value[static_cast<std::size_t>(segment)];
        }
};

}       // namespace mask
}       // namespace board
}       // namespace dctl::core
