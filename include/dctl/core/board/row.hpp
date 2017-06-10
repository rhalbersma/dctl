#pragma once
#include <dctl/core/board/coordinates.hpp>      // to_llo
#include <dctl/core/board/detail/set_filter.hpp>       // set_filter
#include <array>                                // array
#include <cassert>                              // assert
#include <cstddef>                              // size_t

namespace dctl::core {

template<class Board, class Color>
class row
{
        constexpr static auto value = []() {
                std::array<set_t<Board>, Board::height> table{};
                for (auto r = 0; r < Board::height; ++r) {
                        table[static_cast<std::size_t>(r)] = detail::set_filter<Board>([=](auto const sq) {
                                assert(0 <= r); assert(r < Board::height);
                                return to_llo(sq, Board::inner_grid).y == (Color{} == white_c ? r : Board::height - 1 - r);
                        });
                }
                return table;
        }();
public:
        constexpr auto operator()(int const r) const noexcept
        {
                assert(static_cast<std::size_t>(r) < Board::height);
                return value[static_cast<std::size_t>(r)];
        }
};

}       // namespace dctl::core
