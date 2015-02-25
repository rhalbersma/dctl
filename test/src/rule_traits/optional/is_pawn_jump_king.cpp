#include <dctl/rule_traits/optional/is_pawn_jump_king.hpp>      // is_pawn_jump_king
#include <dctl/rules.hpp>                                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(IsPawnJumpKingV)

using VariantsTrue = boost::mpl::vector
<
        checkers::Rules, czech::Rules, frisian::Rules, international::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsTrue)
{
        static_assert(is_pawn_jump_king_v<T>, "");
}

using VariantsFalse = boost::mpl::vector
<
        italian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsFalse)
{
        static_assert(!is_pawn_jump_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
