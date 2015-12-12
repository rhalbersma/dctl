#pragma once

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Russian
{
        static constexpr auto is_long_ranged_king = true;       // 1.4.5

        static constexpr auto is_backward_pawn_jump = true;     // 1.5.3
        static constexpr auto is_passing_promotion = true;      // 1.5.15

        static constexpr auto notation = Notation::algebraic;
        static constexpr auto jumpsep = ':';
};

}       // namespace rules
}       // namespace dctl
