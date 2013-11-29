#pragma once
#include <dctl/grid/predicates.hpp>     // is_square

namespace dctl {
namespace board {

template<class Board>
class Squares
{
private:
        // TODO: replace if constexpr lambdas become available in C++17
        struct lambda
        {
                template<class Square>
                constexpr auto operator()(Square const& sq) noexcept
                {
                        return grid::is_square{}(sq);
                }
        };

        static constexpr typename Board::bit_type value = Board::copy_if(lambda{});

public:
        static constexpr auto mask() noexcept
        {
                return value;
        }
};

template<class Board>
constexpr typename Board::bit_type Squares<Board>::value;

}       // namespace board
}       // namespace dctl

