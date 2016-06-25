#pragma once
#include <dctl/board/detail/coordinates.hpp>    // to_llo
#include <dctl/color.hpp>                       // black, white
#include <dctl/mask/detail/copy_if.hpp>         // copy_if
#include <dctl/utility/fill_array.hpp>          // fill_array
#include <dctl/utility/type_traits.hpp>         // set_t
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl {
namespace mask {

template<class Board>
class row
{
        template<color ToMove>
        struct init
        {
                struct is_row
                {
                        std::size_t const row_;

                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                assert(row_ < Board::height);
                                return board::detail::to_llo(sq, Board::inner_grid).y == (ToMove == color::white ? row_ : Board::height - 1 - row_);
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(std::size_t const row) const noexcept
                {
                        return detail::copy_if<Board>(is_row{row});
                }
        };

        using value_type = std::array<set_t<Board>, Board::height>;

        static constexpr value_type value[] =
        {
                fill_array<Board::height>(init<color::black>{}),
                fill_array<Board::height>(init<color::white>{})
        };

public:
        constexpr auto operator()(color const to_move, std::size_t const row) const noexcept
        {
                assert(row < Board::height);
                return value[xstd::to_underlying_type(to_move)][row];
        }
};

template<class Board>
constexpr typename row<Board>::value_type
row<Board>::value[];

}       // namespace mask
}       // namespace dctl
