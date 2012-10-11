#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/quote.hpp>					// quote1
#include "../../src/mpl/type_traits.hpp"
#include "../../src/mpl/transform.hpp"
#include "../../src/angle/Cyclic.hpp"
#include "../../src/group/primitives.hpp"

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
