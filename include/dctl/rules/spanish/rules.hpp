#pragma once
#include <dctl/rules/spanish/precedence.hpp>    // less
#include <dctl/rule_traits.hpp>

namespace dctl {
namespace spanish {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        using king_range_category = long_ranged_tag;

        // optional
        using jump_precedence = precedence::less;
};

}       // namespace spanish
}       // namespace dctl
