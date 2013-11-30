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
                int const segment_;

                constexpr lambda(int segment) noexcept
                :
                        segment_{segment}
                {}

                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        auto const alpha = segment_ * theta + beta;
                        return grid::is_jump_start{}(rotate(alpha, Board::orientation), sq);
                }
        };

        static constexpr auto init(int segment) noexcept
        {
                return Board::copy_if(lambda{segment});
        }

        using T = typename Board::bit_type;
        static constexpr auto N     = (Board::edge_columns < 2) ? 4 : 8;
        static constexpr auto theta = (Board::edge_columns < 2) ? 90_deg : 45_deg;
        static constexpr auto beta  = (Board::edge_columns < 2) ? 45_deg :  0_deg;
        using table_type = std::array<T, N>;

        static constexpr table_type table = make_array<N>(init);

public:
        static constexpr auto mask(Angle const& alpha) noexcept
        {
                auto const segment = (alpha - beta) / theta;
                return table[static_cast<std::size_t>(segment)];
        }
};

template<class Board>
constexpr Angle JumpStart<Board>::theta;

template<class Board>
constexpr Angle JumpStart<Board>::beta;

template<class Board>
constexpr typename JumpStart<Board>::table_type JumpStart<Board>::table;

}       // namespace board
}       // namespace dctl

