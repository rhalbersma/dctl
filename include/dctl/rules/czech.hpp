#pragma once
#include <dctl/rule_traits.hpp> // algebraic

namespace dctl {
namespace rules {

// http://www.damweb.cz/pravidla/cdfull.html

struct Czech
{
        static constexpr auto is_long_ranged_king = true;       // 3.9
        // static constexpr auto is_backward_pawn_jump = false;    // 4.1

        static constexpr auto jumpsep = ':';
        static constexpr auto notation = Notation::algebraic;   // 2.6

        static constexpr auto is_modality = true;               // 4.14

        struct precedence
        {
                template<class Action>
                constexpr auto operator()(Action const& a) const noexcept
                {
                        return a.is_with_king();
                }
        };
};

}       // namespace rules
}       // namespace dctl
