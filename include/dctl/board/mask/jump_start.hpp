#pragma once
#include <dctl/board/angle.hpp>                 // angle, _deg, rotate, is_diagonal, is_up, is_down, is_left, is_right
#include <dctl/board/detail/coordinates.hpp>    // to_ulo
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/cstddef.hpp>                     // _zu
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class JumpStart
{
        static constexpr auto init(std::size_t const segment) noexcept
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_jump_start
                {
                        std::size_t const segment;

                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                using namespace xstd::support_literals;
                                auto const alpha = rotate(segment * theta + beta, inverse(Board::orientation));
                                auto const offset = is_diagonal(alpha) ? 2_zu : 4_zu;
                                auto const min_x = is_left(alpha) ? offset : 0;
                                auto const max_x = Board::width - (is_right(alpha) ? offset : 0);
                                auto const min_y = is_up(alpha) ? offset : 0;
                                auto const max_y = Board::height - (is_down(alpha) ? offset : 0);
                                auto const coord = detail::to_ulo(sq, Board::inner_grid);
                                return
                                        (min_x <= coord.x && coord.x < max_x) &&
                                        (min_y <= coord.y && coord.y < max_y)
                                ;
                        }
                };

                return make_set_if<Board>(is_jump_start{segment});
        }

        static constexpr angle theta = Board::is_orthogonal_captures ? 45_deg : 90_deg;
        static constexpr angle beta  = Board::is_orthogonal_captures ?  0_deg : 45_deg;
        static constexpr auto N      = Board::is_orthogonal_captures ?      8 :      4;

        using table_type = std::array<set_t<Board>, N>;
        static constexpr table_type table = fill_array<N>(init);

public:
        static constexpr auto mask(angle const alpha) noexcept
        {
                auto const segment = static_cast<std::size_t>((alpha.degrees - beta.degrees) / theta.degrees);
                assert(segment < N);
                return table[segment];
        }
};

template<class Board>
constexpr angle
JumpStart<Board>::theta;

template<class Board>
constexpr angle
JumpStart<Board>::beta;

template<class Board>
constexpr typename JumpStart<Board>::table_type
JumpStart<Board>::table;

}       // namespace board
}       // namespace dctl
