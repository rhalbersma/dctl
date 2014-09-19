#pragma once
#include <dctl/rules/spanish/precedence.hpp>    // less

namespace dctl {
namespace spanish {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_long_ranged_king = true;

        using jump_precedence = precedence::less;
};

}       // namespace spanish
}       // namespace dctl
