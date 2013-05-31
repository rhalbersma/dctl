#include <type_traits>                          // is_same
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/range/scan.hpp>     // scan
#include <dctl/rules/types/range.hpp>           // distance_1, distance_N
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace range {

BOOST_AUTO_TEST_SUITE(RulesTraitsRangeScan)

typedef boost::mpl::vector<
        Checkers, Italian
> VariantsDistance1;

BOOST_AUTO_TEST_CASE_TEMPLATE(Distance1, T, VariantsDistance1)
{
        BOOST_MPL_ASSERT((std::is_same< typename scan<T>::type, distance_1 >));
}

typedef boost::mpl::vector<
        AKWDamme, Czech, Frisian, HHoogland, International, Killer, Pool, Russian, Spanish, Thai
> VariantsDistanceN;

BOOST_AUTO_TEST_CASE_TEMPLATE(DistanceN, T, VariantsDistanceN)
{
        BOOST_MPL_ASSERT((std::is_same< typename scan<T>::type, distance_N >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace range
}       // namespace rules
}       // namespace dctl
