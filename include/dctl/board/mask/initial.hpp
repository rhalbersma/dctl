#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/board/mask/row.hpp>              // row
#include <dctl/color_piece.hpp>                 // black, white
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t, value_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace board {
namespace mask {

template<class Board>
class initial
{
        template<class color>
        struct init
        {
                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(int const rows) const noexcept
                {
                        set_t<Board> result {};
                        for (auto r = 0; r < rows; ++r)
                                result ^= row<Board, color>{}(r);
                        return result;
                }
        };

        static constexpr auto N = Board::height / 2 + 1;
        using value_type = std::array<set_t<Board>, N>;

        static constexpr value_type value[] =
        {
                fill_array<N>(init<black_>{}),
                fill_array<N>(init<white_>{})
        };

public:
        constexpr auto operator()(color const to_move, int const separation) const noexcept
        {
                assert((Board::height - separation) % 2 == 0);
                assert(Board::height % 2 <= separation); assert(separation <= Board::height);
                auto const rows = (Board::height - separation) / 2;
                assert(rows <= Board::height / 2);
                return value[xstd::to_underlying_type(to_move)][static_cast<std::size_t>(rows)];
        }
};

template<class Board>
constexpr value_t<initial<Board>>
initial<Board>::value[];

}       // namespace mask
}       // namespace board
}       // namespace dctl
