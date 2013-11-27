#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/angle.hpp>               // Angle, _deg, rotate
#include <dctl/grid/predicates.hpp>     // is_jump_start
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class jump_start
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                int const i_;
                constexpr lambda(int i) noexcept : i_{i} {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_jump_start{}(rotate(i_ * 45_deg, Board::orientation), sq);
                }
        };

        static constexpr auto init(int n) noexcept
        {
                return Board::copy_if(lambda{n});
        }

        static constexpr std::array<typename Board::bit_type, 8> table = make_array<8>(init);

public:
        constexpr auto operator()(Angle const& direction) noexcept
        {
                return table[static_cast<std::size_t>(direction / 45_deg)];
        }
};

template<class Board>
constexpr std::array<typename Board::bit_type, 8> jump_start<Board>::table;

}       // namespace board
}       // namespace dctl

