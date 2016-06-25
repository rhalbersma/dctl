#pragma once
#include <dctl/board/angle.hpp>                 // angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/detail/coordinates.hpp>    // to_ulo
#include <dctl/mask/detail/copy_if.hpp>         // copy_if
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/cstddef.hpp>                     // _zu
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace mask {

template<class Board>
class jump_start
{
        struct init
        {
                struct is_jump_start
                {
                        std::size_t const segment_;

                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                using namespace xstd::support_literals;
                                auto const alpha = rotate(segment_ * theta + beta, inverse(Board::orientation));
                                auto const offset = is_diagonal(alpha) ? 2_zu : 4_zu;
                                auto const min_x = is_left(alpha) ? offset : 0;
                                auto const max_x = Board::width - (is_right(alpha) ? offset : 0);
                                auto const min_y = is_up(alpha) ? offset : 0;
                                auto const max_y = Board::height - (is_down(alpha) ? offset : 0);
                                auto const coord = board::detail::to_ulo(sq, Board::inner_grid);
                                return
                                        (min_x <= coord.x && coord.x < max_x) &&
                                        (min_y <= coord.y && coord.y < max_y)
                                ;
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(std::size_t const segment) const
                {
                        return detail::copy_if<Board>(is_jump_start{segment});
                }
        };

        static constexpr angle theta = Board::is_orthogonal_captures ? 45_deg : 90_deg;
        static constexpr angle beta  = Board::is_orthogonal_captures ?  0_deg : 45_deg;
        static constexpr auto N      = Board::is_orthogonal_captures ?      8 :      4;
        using value_type = std::array<set_t<Board>, N>;

        static constexpr value_type value = fill_array<N>(init{});

public:
        constexpr auto operator()(angle const alpha) const noexcept
        {
                auto const segment = static_cast<std::size_t>((alpha.degrees - beta.degrees) / theta.degrees);
                assert(segment < N);
                return value[segment];
        }
};

template<class Board>
constexpr angle
jump_start<Board>::theta;

template<class Board>
constexpr angle
jump_start<Board>::beta;

template<class Board>
constexpr typename jump_start<Board>::value_type
jump_start<Board>::value;

}       // namespace mask
}       // namespace dctl
