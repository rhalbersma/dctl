#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/quote.hpp>			// quote1
#include <dctl/mpl/type_traits.hpp>             // is_idempotent
#include <dctl/mpl/transform.hpp>               // inverse
#include <dctl/angle/Cyclic.hpp>                // C8
#include <dctl/group/primitives.hpp>            // set

namespace dctl {
namespace mpl {

BOOST_AUTO_TEST_SUITE(TestDegrees)

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, group::set<angle::C8>::type)
{
        BOOST_MPL_ASSERT(( is_idempotent< boost::mpl::quote1< inverse >, T > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace mpl
}       // namespace dctl
