#pragma once
#include <dctl/board/set_type.hpp>

namespace dctl {
namespace detail {

template<class Board>
class PrecedenceOrdering
{
        get_set_type<Board> piece_order_;
public:
        PrecedenceOrdering() = default;

        template<class Tracker>
        explicit constexpr PrecedenceOrdering(Tracker const& t) noexcept
        :
                piece_order_{t.piece_order()}
        {}

        constexpr auto piece_order() const noexcept
        {
                return piece_order_;
        }
};

}       // namespace detail
}       // namespace dctl
