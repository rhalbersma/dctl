#pragma once
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/board/set_type.hpp>              // set_type

namespace dctl {
namespace board {

template<class Board>
class Squares
{
private:
        // simulate a constexpr lambda (not allowed in C++14)
        struct is_squares
        {
                constexpr auto operator()(int sq) const noexcept
                {
                        return sq < Board::size();
                }
        };

public:
        using value_type = get_set_type<Board>;
        static constexpr value_type value = make_set_if<Board>(is_squares{});
};

template<class Board>
constexpr typename Squares<Board>::value_type
Squares<Board>::value;

template<class Board>
constexpr auto squares_v = Squares<Board>::value;

}       // namespace board
}       // namespace dctl
