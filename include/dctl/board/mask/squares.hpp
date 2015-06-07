#pragma once
#include <dctl/board/mask/make_set_if.hpp>      // make_set_if
#include <dctl/set_type.hpp>                    // set_type

namespace dctl {
namespace board {

template<class Board>
class Squares
{
private:
        // simulate a constexpr lambda (not allowed in C++14)
        struct is_squares
        {
                constexpr auto operator()(std::size_t sq) const noexcept
                {
                        return sq < Board::size();
                }
        };

        using value_type = set_type<Board>;
        static constexpr value_type value = make_set_if<Board>(is_squares{});

public:
        static constexpr auto mask() noexcept
        {
                return value;
        }
};

template<class Board>
constexpr typename Squares<Board>::value_type
Squares<Board>::value;

}       // namespace board
}       // namespace dctl
