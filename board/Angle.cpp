#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>                   // list
#include <boost/mpl/placeholders.hpp>           // _1
using namespace boost::mpl::placeholders;
#include "Transform.hpp"                        // rotate, inverse, mirror_up, mirror_down
#include "../../src/board/Angle.hpp"
#include "../../src/board/Degrees.hpp"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestAngle)

typedef boost::mpl::list<
        angle<degrees::D000>, 
        angle<degrees::D045>, 
        angle<degrees::D090>, 
        angle<degrees::D135>, 
        angle<degrees::D180>, 
        angle<degrees::D225>, 
        angle<degrees::D270>, 
        angle<degrees::D315>
> AngleList;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, AngleList)
{
        check_right_action<T, AngleList>()();
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, AngleList)
{
        BOOST_CHECK((is_idempotent< inverse<_1> , T>::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentMirrorUp, T, AngleList)
{
        BOOST_CHECK((is_idempotent< mirror_up<_1>, T>::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentMirrorDown, T, AngleList)
{
        BOOST_CHECK((is_idempotent< mirror_down<_1>, T>::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
