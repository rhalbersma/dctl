#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/quote.hpp>                  // quote1
#include "../../src/board/Angle.hpp"
#include "../../src/board/Cyclic.hpp"
#include "../../src/board/Group.hpp"

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(TestAngle)

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, set<C8>::type)
{
        BOOST_CHECK((is_right_action< T, C1 >::value));
        BOOST_CHECK((is_right_action< T, C2 >::value));
        BOOST_CHECK((is_right_action< T, C4 >::value));
        BOOST_CHECK((is_right_action< T, C8 >::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, set<C8>::type)
{
        BOOST_CHECK((is_idempotent< boost::mpl::quote1< inverse >, T>::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentMirrorUp, T, set<C8>::type)
{
        BOOST_CHECK((is_idempotent< boost::mpl::quote1< mirror_up >, T>::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentMirrorDown, T, set<C8>::type)
{
        BOOST_CHECK((is_idempotent< boost::mpl::quote1< mirror_down >, T>::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
