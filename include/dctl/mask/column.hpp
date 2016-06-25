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
class column
{
        template<color ToMove>
        struct init
        {
                struct is_column
                {
                        std::size_t const column_;

                        // simulate a constexpr lambda (allowed in C++17)
                        constexpr auto operator()(std::size_t const sq) const noexcept
                        {
                                assert(column_ < Board::width);
                                return board::detail::to_llo(sq, Board::inner_grid).x == (ToMove == color::white ? column_ : Board::width - 1 - column_);
                        }
                };

                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(std::size_t const column) const noexcept
                {
                        return detail::copy_if<Board>(is_column{column});
                }
        };

        using value_type = std::array<set_t<Board>, Board::width>;

        static constexpr value_type value[] =
        {
                fill_array<Board::width>(init<color::black>{}),
                fill_array<Board::width>(init<color::white>{})
        };

public:
        constexpr auto operator()(color const to_move, std::size_t const column) const noexcept
        {
                assert(column < Board::width);
                return value[xstd::to_underlying_type(to_move)][column];
        }
};

template<class Board>
constexpr typename column<Board>::value_type
column<Board>::value[];

}       // namespace mask
}       // namespace dctl
