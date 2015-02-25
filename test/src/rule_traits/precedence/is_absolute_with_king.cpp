#include <dctl/rule_traits/precedence/is_absolute_with_king.hpp>        // is_absolute_with_king
#include <dctl/rules.hpp>                                               // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                                         // vector
#include <boost/test/test_case_template.hpp>                            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace precedence {

BOOST_AUTO_TEST_SUITE(IsAbsoluteWithKingV)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
{
        static_assert(!is_absolute_with_king_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        czech::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
{
        static_assert(is_absolute_with_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace prececence
}       // namespace dctl
