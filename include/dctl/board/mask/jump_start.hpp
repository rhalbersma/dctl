#pragma once
#include <array>                        // array
#include <cstddef>                      // size_t
#include <dctl/angle.hpp>               // Angle, _deg, rotate
#include <dctl/grid/predicates.hpp>     // is_jump_start
#include <dctl/utility/make_array.hpp>  // make_array

namespace dctl {
namespace board {

template<class Board>
class JumpStart
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                int const direction_;

                constexpr lambda(int direction) noexcept
                :
                        direction_{direction}
                {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_jump_start{}(rotate(direction_ * 45_deg, Board::orientation), sq);
                }
        };

        static constexpr auto init(int direction) noexcept
        {
                return Board::copy_if(lambda{direction});
        }

        static constexpr std::array<typename Board::bit_type, 8> table = make_array<8>(init);

public:
        static constexpr auto mask(Angle const& direction) noexcept
        {
                return table[static_cast<std::size_t>(direction / 45_deg)];
        }
};

template<class Board>
constexpr std::array<typename Board::bit_type, 8> JumpStart<Board>::table;

}       // namespace board
}       // namespace dctl

