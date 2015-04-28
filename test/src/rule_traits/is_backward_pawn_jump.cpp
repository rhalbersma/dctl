#include <dctl/rule_traits.hpp>                 // is_backward_pawn_jump
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {

BOOST_AUTO_TEST_SUITE(IsBackwardPawnJumpV)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, czech::Rules, italian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
{
        static_assert(!is_backward_pawn_jump_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        frisian::Rules, international::Rules, pool::Rules, russian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
{
        static_assert(is_backward_pawn_jump_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
