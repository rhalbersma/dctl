#pragma once
#include <dctl/core/board/angle.hpp>            // angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/core/board/coordinates.hpp>      // to_ulo
#include <dctl/core/board/detail/set_filter.hpp>       // set_filter
#include <dctl/util/fill_array.hpp>             // fill_array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {

template<class Board>
class jump_start
{
        constexpr static auto theta = Board::is_orthogonal_jump ? 45_deg : 90_deg;
        constexpr static auto beta  = Board::is_orthogonal_jump ?  0_deg : 45_deg;
        constexpr static auto N      = Board::is_orthogonal_jump ?      8 :      4;
        constexpr static auto value = fill_array<N>([](int const segment){
                return detail::set_filter<Board>([=](auto const sq){
                        auto const alpha = rotate(segment * theta + beta, inverse(Board::orientation));
                        auto const offset = is_diagonal(alpha) ? 2 : 4;
                        auto const min_x = is_left(alpha) ? offset : 0;
                        auto const max_x = Board::width - (is_right(alpha) ? offset : 0);
                        auto const min_y = is_up(alpha) ? offset : 0;
                        auto const max_y = Board::height - (is_down(alpha) ? offset : 0);
                        auto const coord = to_ulo(sq, Board::inner_grid);
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

}       // namespace dctl::core
