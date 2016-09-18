#include <dctl/rules/checkers.hpp>      // Checkers
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, is_long_ranged_king, is_nontrivial_precedence
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesCheckers)

using T = Checkers;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_or_v<T>);
        static_assert(!is_long_ranged_king_or_v<T>);
        static_assert(!is_nontrivial_precedence_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
