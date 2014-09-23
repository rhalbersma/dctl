#include <dctl/rule_traits/optional/initial_gap.hpp>    // initial_gap
#include <dctl/rules.hpp>                               // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsPawnJumpKing)

using Variants2 = boost::mpl::vector
<
        checkers::Rules, czech::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(InitialGapVIs2, T, Variants2)
{
        static_assert(initial_gap_v<T> == 2, "");
}

using Variants4 = boost::mpl::vector
<
        thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(InitialGapVIs4, T, Variants4)
{
        static_assert(initial_gap_v<T> == 4, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
