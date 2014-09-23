#include <dctl/rules/checkers.hpp>      // Checkers
#include <dctl/rule_traits.hpp>         // is_backward_pawn, is_jump_precedence, is_long_ranged_king
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace checkers {

BOOST_AUTO_TEST_SUITE(RulesVariantsCheckers)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(!is_jump_precedence_v<T>, "");
        static_assert(!is_long_ranged_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace checkers
}       // namespace dctl
