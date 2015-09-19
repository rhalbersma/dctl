#pragma once
#include <dctl/piece.hpp>       // king
#include <dctl/rule_traits.hpp> // long_ranged_tag

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html

struct Czech
{
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        static constexpr auto jumpsep = ':';
        static constexpr auto notation = Notation::algebraic;

        static constexpr auto is_modality = true;

        struct equal_to
        {
                template<class Action1, class Action2>
                constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
                {
                        return lhs.is_with(Piece::king) == rhs.is_with(Piece::king);
                }
        };

        struct less
        {
                template<class Action1, class Action2>
                constexpr auto operator()(Action1 const& lhs, Action2 const& rhs) const noexcept
                {
                        return lhs.is_with(Piece::king) < rhs.is_with(Piece::king);
                }
        };
};

}       // namespace rules
}       // namespace dctl
