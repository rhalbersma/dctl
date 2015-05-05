#pragma once
#include <dctl/rule_traits.hpp> // long_ranged_tag, passing_promotion_tag

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Russian
{
        static constexpr auto is_backward_pawn_jump = true;     // 1.5.3
        using king_range_category = long_ranged_tag;            // 1.4.5

        static constexpr auto notation = Notation::algebraic;
        static constexpr auto jumpsep = ':';
        using promotion_category = passing_promotion_tag;       // 1.5.15
};

}       // namespace rules
}       // namespace dctl
