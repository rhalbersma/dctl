#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/quote.hpp>                  // quote1
#include "../../src/group/action.hpp"
#include "../../src/group/primitives.hpp"
#include "../../src/angle/Cyclic.hpp"

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(TestDegrees)

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRightActionC8, T, set<angle::C8>::type)
{
        BOOST_MPL_ASSERT(( is_right_action< T, angle::C1 > ));
        BOOST_MPL_ASSERT(( is_right_action< T, angle::C2 > ));
        BOOST_MPL_ASSERT(( is_right_action< T, angle::C4 > ));
        BOOST_MPL_ASSERT(( is_right_action< T, angle::C8 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
