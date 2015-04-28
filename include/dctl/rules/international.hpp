#pragma once
#include <dctl/rule_traits.hpp> // long_ranged_tag

namespace dctl {
namespace international {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct Rules
{
        // required
        static constexpr auto is_backward_pawn_jump = true;     // 4.1
        using king_range_category = long_ranged_tag;            // 3.9

        // draw
        static constexpr auto max_repetitions = 3;              // 6.1
        static constexpr auto max_reversible_moves = 50;        // 6.2

        // precedence
        struct equal_to
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) const noexcept
                {
                        return lhs.num_captured() == rhs.num_captured();
                }
        };

        struct less
        {
                template<class Move>
                constexpr auto operator()(Move const& lhs, Move const& rhs) const noexcept
                {
                        return lhs.num_captured() < rhs.num_captured();
                }
        };
};

}       // namespace international

namespace brazilian  = international;
namespace philippine = international;
namespace canadian   = international;
namespace srilankan  = international;

}       // namespace dctl
