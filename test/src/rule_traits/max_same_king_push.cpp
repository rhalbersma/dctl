#include <dctl/rule_traits.hpp>                 // max_same_king_push
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(MaxSameKingPushV)

using VariantsZero = boost::mpl::vector
<
        checkers::Rules, czech::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsZero, T, VariantsZero)
{
        static_assert(max_same_king_push_v<T> == 0, "");
}

using VariantsThree = boost::mpl::vector
<
        frisian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsThree, T, VariantsThree)
{
        static_assert(max_same_king_push_v<T> == 3, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
