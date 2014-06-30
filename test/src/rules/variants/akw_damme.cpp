#include <dctl/rules/variants/akw_damme.hpp>    // AKWDamme
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsAKWDamme)

using T = AKWDamme;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT((std::is_same< range::halt<T>::type, range::distance_1 >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
