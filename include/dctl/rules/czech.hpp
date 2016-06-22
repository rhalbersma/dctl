#pragma once
#include <dctl/board.hpp>       // checkers

namespace dctl {
namespace rules {
namespace block_adl {

// http://www.damweb.cz/pravidla/cdfull.html
struct czech
{
        using board_type = board::checkers;
        static constexpr auto is_algebraic_notation = true;     // 2.6
        static constexpr auto jumpsep = ':';

        static constexpr auto is_long_ranged_king = true;       // 3.9
        static constexpr auto is_backward_pawn_jump = false;    // 4.1
        static constexpr auto is_modality_precedence = true;    // 4.14

        struct tuple_type
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.is_with_king();
                }
        };
};

}       // namespace block_adl

using block_adl::czech;

}       // namespace rules
}       // namespace dctl
