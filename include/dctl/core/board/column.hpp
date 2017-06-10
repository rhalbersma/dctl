#pragma once
#include <dctl/core/board/coordinates.hpp>      // to_llo
#include <dctl/core/board/detail/set_filter.hpp>       // set_filter
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {

template<class Board, class Color>
class column
{
        constexpr static auto value = []() {
                std::array<set_t<Board>, Board::width> table{};
                for (auto c = 0; c < Board::width; ++c) {
                        table[static_cast<std::size_t>(c)] = detail::set_filter<Board>([=](auto const sq){
                                assert(0 <= c); assert(c < Board::width);
                                return to_llo(sq, Board::inner_grid).x == (Color{} == white_c ? c : Board::width - 1 - c);
                        });
                }
                return table;
        }();
public:
        constexpr auto operator()(int const c) const noexcept
        {
                assert(static_cast<std::size_t>(c) < Board::width);
                return value[static_cast<std::size_t>(c)];
        }
};

}       // namespace dctl::core
