#pragma once
#include <dctl/utility/type_traits.hpp>

namespace dctl {
namespace detail {

template<class Board>
class PrecedenceOrdering
{
        set_t<Board> piece_order_ {};
public:
        PrecedenceOrdering() = default;

        constexpr auto piece_order() const noexcept
        {
                return piece_order_;
        }
};

}       // namespace detail
}       // namespace dctl
