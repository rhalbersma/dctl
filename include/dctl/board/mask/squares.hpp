#pragma once
#include <dctl/board/mask/copy_if.hpp>  // copy_if

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

        using value_type = typename Board::set_type;

        static constexpr value_type value = copy_if(Board{}, lambda{});

public:
        static constexpr auto mask() noexcept
        {
                return value;
        }
};

template<class Board>
constexpr typename Squares<Board>::value_type Squares<Board>::value;

template<class Board>
constexpr auto squares = Squares<Board>::mask();

}       // namespace board
}       // namespace dctl
