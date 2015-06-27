#pragma once
#include <dctl/board/coordinates.hpp>           // ulo_from_sq
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/player.hpp>                       // black, white
#include <dctl/set_type.hpp>                    // set_type
#include <dctl/utility/make_array.hpp>          // make_array
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class Initial
{
        template<Player ToMove>
        static constexpr auto init(int rows) noexcept
        {
                // simulate a constexpr lambda (not allowed in C++14)
                struct is_initial
                {
                        Player const to_move;
                        int const rows_;

                        constexpr auto operator()(std::size_t sq) const noexcept
                        {
                                auto const separation = Board::height() - 2 * rows_;
                                auto const y = to_llo(sq, Board::inner_grid).y();
                                auto const min_y = to_move == Player::white ? 0 : Board::height() - (Board::height() - separation) / 2;
                                auto const max_y = to_move == Player::white ? (Board::height() - separation) / 2 : Board::height();
                                return min_y <= y && y < max_y;
                        }
                };

                return make_set_if<Board>(is_initial{ToMove, rows});
        }

        static constexpr auto N = Board::height() / 2 + 1;
        using table_type = std::array<set_type<Board>, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Player::black>),
                make_array<N>(init<Player::white>)
        };

public:
        static constexpr auto mask(Player c, int separation) noexcept
        {
                assert((Board::height() - separation) % 2 == 0);
                assert(Board::height() % 2 <= separation && separation <= Board::height());
                auto const rows = (Board::height() - separation) / 2;
                assert(0 <= rows && rows <= Board::height() / 2);
                return table[xstd::to_underlying_type(c)][static_cast<std::size_t>(rows)];
        }
};

template<class Board>
constexpr typename Initial<Board>::table_type
Initial<Board>::table[];

}       // namespace board
}       // namespace dctl
