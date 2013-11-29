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
                bool const c_;
                int const n_;
                constexpr lambda(bool c, int n) noexcept : c_{c}, n_{n} {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_row{}(c_, n_, sq);
                }
        };

        template<bool Color>
        static constexpr auto init(int n) noexcept
        {
                return Board::copy_if(lambda{Color, n});
        }

        static constexpr std::array<typename Board::bit_type, 9> table[] =
        {
                make_array<9>(init<Side::black>),
                make_array<9>(init<Side::white>)
        };

public:
        static constexpr auto mask(bool color, int separation) noexcept
        {
                return table[color][static_cast<std::size_t>(separation)];
        }
};

template<class Board>
constexpr std::array<typename Board::bit_type, 9> Initial<Board>::table[];

}       // namespace board
}       // namespace dctl

