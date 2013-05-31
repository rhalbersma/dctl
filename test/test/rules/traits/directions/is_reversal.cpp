#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/directions/is_reversal.hpp> // is_reversal
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace directions {

BOOST_AUTO_TEST_SUITE(RulesTraitsDirectionsIsReversal)

typedef boost::mpl::vector<
        Thai
> VariantsTrueType;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsReversalTrueType, T, VariantsTrueType)
{
        BOOST_MPL_ASSERT(( is_reversal<T> ));
}

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish
> VariantsFalseType;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsReversalFalseType, T, VariantsFalseType)
{
        BOOST_MPL_ASSERT_NOT(( is_reversal<T> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace directions
}       // namespace rules
}       // namespace dctl
