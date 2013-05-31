#pragma once
#include <type_traits>                                  // integral_constant
#include <dctl/rules/variants/international_fwd.hpp>    // International
#include <dctl/rules/types/range.hpp>                   // distance_N
#include <dctl/rules/types/directions.hpp>              // diag
#include <dctl/rules/types/precedence.hpp>              // quantity
#include <dctl/successor/value/international.hpp>       // Value (International specialization)

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc

struct International
{
        // main rules
        typedef range::distance_N range_king;                           // 3.9
        typedef directions::diag directions_pawn_jump;                  // 4.1
        typedef precedence::quantity precedence_jump;                   // 4.13

        // drawing rules
        typedef std::integral_constant<int,  3> max_repetitions;        // 6.1
        typedef std::integral_constant<int, 50> max_reversible_moves;   // 6.2
        typedef std::integral_constant<int, 32> max_3v1_moves;          // 6.3
        typedef std::integral_constant<int, 10> max_2v1_moves;          // 6.4
};

}       // namespace rules
}       // namespace dctl
