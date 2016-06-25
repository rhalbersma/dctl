#pragma once
#include <dctl/mask/detail/copy_if.hpp> // copy_if
#include <dctl/utility/type_traits.hpp> // set_t
#include <cstddef>                      // size_t

namespace dctl {
namespace mask {

template<class Board>
class squares
{
        struct is_square
        {
                // simulate a constexpr lambda (allowed in C++17)
                constexpr auto operator()(std::size_t const sq) const noexcept
                {
                        return sq < Board::size();
                }
        };

        static constexpr auto value = detail::copy_if<Board>(is_square{});

public:
        constexpr auto operator()() const noexcept
        {
                return value;
        }
};

template<class Board>
constexpr auto squares_v = squares<Board>{}();

}       // namespace mask
}       // namespace dctl
