#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/quote.hpp>                  // quote1
#include "../../src/board/Degrees.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/board/Traits.hpp"
#include "../../src/group/action.hpp"
#include "../../src/group/primitives.hpp"
#include "../../src/mpl/type_traits.hpp"

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(TestAngle)

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, group::set<C8>::type)
{
        BOOST_MPL_ASSERT(( mpl::is_idempotent< boost::mpl::quote1< inverse >, T > ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, group::set<C8>::type)
{
        BOOST_MPL_ASSERT(( group::is_right_action< T, C1 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, C2 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, C4 > ));
        BOOST_MPL_ASSERT(( group::is_right_action< T, C8 > ));
}

BOOST_AUTO_TEST_CASE(MirrorUp)
{
        BOOST_MPL_ASSERT(( is_up< mirror< D045, D090 > > ));
        BOOST_MPL_ASSERT(( is_up< mirror< D135, D090 > > ));
}

BOOST_AUTO_TEST_CASE(MirrorDown)
{
        BOOST_MPL_ASSERT(( is_down< mirror< D225, D270 > > ));
        BOOST_MPL_ASSERT(( is_down< mirror< D315, D270 > > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
