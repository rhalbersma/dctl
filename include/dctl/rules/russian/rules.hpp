#pragma once
#include <dctl/successor/value.hpp>     // Value

namespace dctl {
namespace russian {

// http://www.shashist.ru/kodeks/kodeks2004.doc

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;     // 1.5.3
        static constexpr auto is_jump_precedence = false;       // 1.5.14
        static constexpr auto is_long_ranged_king = true;       // 1.4.5

        using value_type = successor::Value;

        // optional
        static constexpr auto is_en_passant_promotion = true;   // 1.5.15
};

}       // namespace russian
}       // namespace dctl
