#include <dctl/core/rules/thai.hpp>          // thai
#include <dctl/core/rule_traits.hpp>         // is_backward_pawn_jump, is_long_ranged_king, is_trivial_precedence, initial_position_gap, is_land_behind_piece, is_passing_capture, jumpsep
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(Rulesthai)

using T = thai;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(initial_position_gap_v<T> == 4);
        static_assert(is_land_behind_piece_v<T>);
        static_assert(is_passing_capture_v<T>);
        static_assert(jumpsep_v<T> == '-');

        static_assert(is_trivial_precedence_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl