#include <dctl/rule_traits.hpp>                 // initial_gap
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(InitialStateGap)

using VariantsTwo = boost::mpl::vector
<
        checkers, czech, frisian, international, italian, pool, russian, spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTwo, T, VariantsTwo)
{
        static_assert(initial_position_gap_or_v<T> == 2);
}

using VariantsFour = boost::mpl::vector
<
        thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsFour, T, VariantsFour)
{
        static_assert(initial_position_gap_or_v<T> == 4);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
