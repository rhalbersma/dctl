#include <dctl/rule_traits/required/is_backward_pawn_jump.hpp>  // is_backward_pawn_jump_v
#include <dctl/rules.hpp>                                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsBackwardPawnJump)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, czech::Rules, italian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsBackwardPawnJumpV, T, VariantsFalse)
{
        static_assert(!is_backward_pawn_jump_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        frisian::Rules, international::Rules, pool::Rules, russian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsBackwardPawnJumpV, T, VariantsTrue)
{
        static_assert(is_backward_pawn_jump_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
