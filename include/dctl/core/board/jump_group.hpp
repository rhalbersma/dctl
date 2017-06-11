#pragma once
#include <dctl/core/board/detail/set_filter.hpp>        // set_filter
#include <dctl/core/board/coordinates.hpp>              // to_llo
#include <dctl/util/type_traits.hpp>                    // set_t, value_t
#include <xstd/cstdlib.hpp>                             // euclidean_div
#include <array>                                        // array
#include <cassert>                                      // assert

namespace dctl::core {

template<class Board>
class jump_group
{
        template<int FromSquare>
        struct init
        {
                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()() const noexcept
                {
                        return detail::set_filter<Board>([](int const dest_sq){
                                auto const from_coord = to_llo(FromSquare, Board::inner_grid);
                                auto const dest_coord = to_llo(dest_sq   , Board::inner_grid);
                                auto const delta_x = xstd::euclidean_div(static_cast<int>(from_coord.x) - static_cast<int>(dest_coord.x), 4).rem;
                                auto const delta_y = xstd::euclidean_div(static_cast<int>(from_coord.y) - static_cast<int>(dest_coord.y), 4).rem;
                                return
                                        (delta_x == 0 && delta_y == 0) ||
                                        (delta_x == 2 && delta_y == 2)
                                ;
                        });
                }
        };

        constexpr static auto table = std::array<set_t<Board>, 4>
        {{
                init<Board::edge_le() + 0>{}(),
                init<Board::edge_le() + 1>{}(),
                init<Board::edge_lo() + 0>{}(),
                init<Board::edge_lo() + 1>{}()
        }};

public:
        constexpr auto operator()(int const n) const noexcept
        {
                assert(static_cast<std::size_t>(n) < 4);
                return table[static_cast<std::size_t>(n)];
        }
};

}       // namespace dctl::core
