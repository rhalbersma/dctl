#include <dctl/rule_traits.hpp>                 // initial_gap_v
#include <dctl/rules/variants.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsIsPawnJumpKing)

using Variants2 = boost::mpl::vector<
        Checkers, Czech, Frisian, International, Italian, Killer, Pool, Russian, Spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(InitialGapVIs2, T, Variants2)
{
        static_assert(initial_gap_v<T> == 2, "");
}

using Variants4 = boost::mpl::vector<
        Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(InitialGapVIs4, T, Variants4)
{
        static_assert(initial_gap_v<T> == 4, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
