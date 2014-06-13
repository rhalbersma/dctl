#include <dctl/rules/traits/is_reversible_king_jump_direction.hpp> // is_reversible_king_jump_direction
#include <dctl/rules/variants.hpp>
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                                  // is_same

namespace dctl {
namespace rules {
namespace directions {

BOOST_AUTO_TEST_SUITE(RulesTraitsDirectionsIsReversal)

using VariantsTrueType = boost::mpl::vector
<
        Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(TrueType, T, VariantsTrueType)
{
        BOOST_MPL_ASSERT(( is_reversible_king_jump_direction_t<T> ));
}

using VariantsFalseType = boost::mpl::vector
<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(FalseType, T, VariantsFalseType)
{
        BOOST_MPL_ASSERT_NOT(( is_reversible_king_jump_direction_t<T> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace directions
}       // namespace rules
}       // namespace dctl
