#pragma once
#include <array>                        // array
#include <cassert>                      // assert
#include <cstddef>                      // size_t
#include <dctl/grid/predicates.hpp>     // is_initial
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class Initial
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                bool const color_;
                int const rows_;

                constexpr lambda(bool color, int rows) noexcept
                :
                        color_{color},
                        rows_{rows}
                {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        auto const separation = Board::height - 2 * rows_;
                        assert((Board::height % 2) <= separation && separation <= Board::height);
                        return grid::is_initial{}(color_, separation, sq);
                }
        };

        template<bool Color>
        static constexpr auto init(int separation) noexcept
        {
                return Board::copy_if(lambda{Color, separation});
        }

        using T = typename Board::bit_type;
        static constexpr auto N = (Board::height) / 2 + 1;
        using table_type = std::array<T, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Side::black>),
                make_array<N>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int separation) noexcept
        {
                assert((Board::height - separation) % 2 == 0);
                auto const rows = (Board::height - separation) / 2;
                assert(0 <= rows && rows <= (Board::height / 2));
                return table[color][static_cast<std::size_t>(rows)];
        }
};

template<class Board>
constexpr typename Initial<Board>::table_type Initial<Board>::table[];

}       // namespace board
}       // namespace dctl

