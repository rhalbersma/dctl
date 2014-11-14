#pragma once
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/grid/coordinates.hpp>            // ulo_from_sq
#include <dctl/position/color.hpp>              // black, white
#include <dctl/utility/make_array.hpp>          // make_array
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {

template<class Board>
class Initial
{
private:
        // simulate a constexpr lambda (not allowed in C++14)
        struct lambda
        {
                bool const color_;
                int const rows_;

                template<class Square>
                constexpr auto operator()(Square const& sq) const noexcept
                {
                        auto const separation = Board::height - 2 * rows_;
                        auto const y = get_y(swap_llo_ulo(ulo_from_sq(sq)));
                        auto const min_y = color_ == Color::white ? 0 : Board::height - (Board::height - separation) / 2;
                        auto const max_y = color_ == Color::white ? (Board::height - separation) / 2 : Board::height;
                        return min_y <= y && y < max_y;
                }
        };

        template<bool Color>
        static constexpr auto init(int rows) noexcept
        {
                return make_set_if<Board>(lambda{Color, rows});
        }

        static constexpr auto N = Board::height / 2 + 1;
        using Set = typename Board::set_type;
        using table_type = std::array<Set, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Color::black>),
                make_array<N>(init<Color::white>)
        };

public:
        static constexpr auto mask(bool color, int separation) noexcept
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation && separation <= Board::height);
                auto const rows = (Board::height - separation) / 2;
                assert(0 <= rows && rows <= Board::height / 2);
                return table[color][static_cast<std::size_t>(rows)];
        }
};

template<class Board>
constexpr typename Initial<Board>::table_type
Initial<Board>::table[];

}       // namespace board
}       // namespace dctl
