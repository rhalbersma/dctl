#pragma once

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
                constexpr auto operator()(Square const& sq) const noexcept
                {
                        using Grid = typename Square::grid_type;
                        return 0 <= sq.value() && sq.value() < Grid::size;
                }
        };

        using value_type = typename Board::bit_type;

        static constexpr value_type value = Board::copy_if(lambda{});

public:
        static constexpr auto mask() noexcept
        {
                return value;
        }
};

template<class Board>
constexpr typename Squares<Board>::value_type Squares<Board>::value;

}       // namespace board
}       // namespace dctl
