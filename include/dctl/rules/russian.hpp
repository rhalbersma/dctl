#pragma once
#include <dctl/rules/russian_fwd.hpp>   // Russian

namespace dctl {
namespace rules {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Russian
{
        // required
        static constexpr auto is_backward_pawn_jump = true;     // 1.5.3
        static constexpr auto is_jump_precedence = false;       // 1.5.14
        static constexpr auto is_long_ranged_king = true;       // 1.4.5

        // optional
        static constexpr auto is_en_passant_promotion = true;   // 1.5.15
};

}       // namespace rules
}       // namespace dctl
