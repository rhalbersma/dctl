#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <dctl/group/axioms.hpp>        // is_realized
#include <dctl/angle/Cyclic.hpp>	// C1, C2, C4, C8

namespace dctl {
namespace group {
namespace axioms {

BOOST_AUTO_TEST_SUITE(TestAxioms)

BOOST_AUTO_TEST_CASE(testIsRealized)
{
        BOOST_MPL_ASSERT(( is_realized<angle::C1> ));
        BOOST_MPL_ASSERT(( is_realized<angle::C2> ));
        BOOST_MPL_ASSERT(( is_realized<angle::C4> ));
        BOOST_MPL_ASSERT(( is_realized<angle::C8> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace axioms
}       // namespace group
}       // namespace dctl
