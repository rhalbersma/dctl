#pragma once
#include <dctl/rules/spanish/value.hpp> // Value

namespace dctl {
namespace spanish {

// http://fpdamas.home.sapo.pt/regrasclass.htm

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = false;
        static constexpr auto is_jump_precedence = true;
        static constexpr auto is_long_ranged_king = true;

        using value_type = Value;
};

}       // namespace spanish
}       // namespace dctl
