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

        template<class State>
        explicit constexpr PrecedenceOrdering(State const&) noexcept
        :
                piece_order_{0}
        {}

        template<class... State>
        constexpr auto piece_order(State const&...) const noexcept
        {
                return piece_order_;
        }
};

}       // namespace detail
}       // namespace dctl
