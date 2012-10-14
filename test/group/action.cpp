#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/quote.hpp>                  // quote1
#include "../../src/group/action.hpp"           // is_realized
#include "../../src/group/primitives.hpp"       // set
#include "../../src/angle/Cyclic.hpp"           // C1, C2, C4, C8

namespace dctl {
namespace group {
namespace action {

BOOST_AUTO_TEST_SUITE(TestRightAction)

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRealized, T, set<angle::C8>::type)
{
        BOOST_MPL_ASSERT(( is_realized< T, angle::C1 > ));
        BOOST_MPL_ASSERT(( is_realized< T, angle::C2 > ));
        BOOST_MPL_ASSERT(( is_realized< T, angle::C4 > ));
        BOOST_MPL_ASSERT(( is_realized< T, angle::C8 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace action
}       // namespace group
}       // namespace dctl
