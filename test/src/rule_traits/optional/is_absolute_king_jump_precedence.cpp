#include <dctl/rule_traits/optional/is_absolute_king_jump_precedence.hpp>       // is_absolute_king_jump_precedence
#include <dctl/rules.hpp>                                                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                                 // vector
#include <boost/test/test_case_template.hpp>                                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsAbsoluteKingJumpPrecedence)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsAbsoluteKingJumpPrecedenceV, T, VariantsFalse)
{
        static_assert(!is_absolute_king_jump_precedence_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        czech::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsAbsoluteKingJumpPrecedenceV, T, VariantsTrue)
{
        static_assert(is_absolute_king_jump_precedence_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
