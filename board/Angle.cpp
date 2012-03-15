#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/quote.hpp>                  // quote1
#include "../../src/board/Angle.hpp"
#include "../../src/board/Degrees.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/board/Traits.hpp"
#include "../../src/group/action.hpp"
#include "../../src/group/primitives.hpp"
#include "../../src/mpl/type_traits.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestAngle)

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, group::set<C8>::type)
{
        BOOST_CHECK((mpl::is_idempotent< boost::mpl::quote1< inverse >, T >::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, group::set<C8>::type)
{
        BOOST_CHECK((group::is_right_action< T, C1 >::value));
        BOOST_CHECK((group::is_right_action< T, C2 >::value));
        BOOST_CHECK((group::is_right_action< T, C4 >::value));
        BOOST_CHECK((group::is_right_action< T, C8 >::value));
}

BOOST_AUTO_TEST_CASE(MirrorUp)
{
        BOOST_CHECK((board::is_up< mirror< angle<degrees::D045>, angle<degrees::D090> > >::value));
        BOOST_CHECK((board::is_up< mirror< angle<degrees::D135>, angle<degrees::D090> > >::value));
}

BOOST_AUTO_TEST_CASE(MirrorDown)
{
        BOOST_CHECK((board::is_down< mirror< angle<degrees::D225>, angle<degrees::D270> > >::value));
        BOOST_CHECK((board::is_down< mirror< angle<degrees::D315>, angle<degrees::D270> > >::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
