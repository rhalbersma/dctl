#include <type_traits>                          // is_same
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/range/land.hpp>     // land
#include <dctl/rules/types/range.hpp>           // distance_1, distance_N
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace range {

BOOST_AUTO_TEST_SUITE(RulesTraitsRangeLand)

typedef boost::mpl::vector<
        Checkers, Italian, Thai
> VariantsDistance1;

BOOST_AUTO_TEST_CASE_TEMPLATE(LandDistance1, T, VariantsDistance1)
{
        BOOST_MPL_ASSERT((std::is_same< typename land<T>::type, distance_1 >));
}

typedef boost::mpl::vector<
        AKWDamme, Czech, Frisian, HHoogland, International, Killer, Pool, Russian, Spanish
> VariantsDistanceN;

BOOST_AUTO_TEST_CASE_TEMPLATE(LandDistanceN, T, VariantsDistanceN)
{
        BOOST_MPL_ASSERT((std::is_same< typename land<T>::type, distance_N >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace range
}       // namespace rules
}       // namespace dctl
