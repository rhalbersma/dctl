#pragma once
#include <array>                        // array
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
                int const separation_;

                constexpr lambda(bool color, int separation) noexcept
                :
                        color_{color},
                        separation_{separation}
                {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_initial{}(color_, separation_, sq);
                }
        };

        template<bool Color>
        static constexpr auto init(int separation) noexcept
        {
                return Board::copy_if(lambda{Color, separation});
        }

        using T = typename Board::bit_type;
        static constexpr auto N = Board::height / 2;
        using table_type = std::array<T, N>;

        static constexpr table_type table[] =
        {
                make_array<N>(init<Side::black>),
                make_array<N>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int separation) noexcept
        {
                return table[color][static_cast<std::size_t>(separation)];
        }
};

template<class Board>
constexpr typename Initial<Board>::table_type Initial<Board>::table[];

}       // namespace board
}       // namespace dctl

