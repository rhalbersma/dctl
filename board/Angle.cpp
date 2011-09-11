#include <boost/test/unit_test.hpp> 
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>
#include <boost/type_traits/is_same.hpp>
#include "../../src/board/Angle.h"
#include "../../src/board/Transform.h"

namespace dctl {

BOOST_AUTO_TEST_SUITE(TestAngle)

typedef boost::mpl::list<
        Angle<0>, Angle<1>, Angle<2>, Angle<3>,
        Angle<4>, Angle<5>, Angle<6>, Angle<7> 
> AngleList;

BOOST_AUTO_TEST_CASE_TEMPLATE(Inverse, T, AngleList)
{
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::R045>::type, Degrees::L045>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::L045>::type, Degrees::R045>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::R090>::type, Degrees::L090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::L090>::type, Degrees::R090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::R135>::type, Degrees::L135>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::L135>::type, Degrees::R135>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::D180>::type, Degrees::D180>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<T, rotate<rotate<T, Degrees::D360>::type, Degrees::D360>::type>::value));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(Square, T, AngleList)
{
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::L090>::type, rotate<rotate<T, Degrees::L045>::type, Degrees::L045>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::R090>::type, rotate<rotate<T, Degrees::R045>::type, Degrees::R045>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D180>::type, rotate<rotate<T, Degrees::L090>::type, Degrees::L090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D180>::type, rotate<rotate<T, Degrees::R090>::type, Degrees::R090>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::R090>::type, rotate<rotate<T, Degrees::L135>::type, Degrees::L135>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::L090>::type, rotate<rotate<T, Degrees::R135>::type, Degrees::R135>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D360>::type, rotate<rotate<T, Degrees::D180>::type, Degrees::D180>::type>::value));
        BOOST_CHECK_EQUAL(true, (boost::is_same<rotate<T, Degrees::D360>::type, rotate<rotate<T, Degrees::D360>::type, Degrees::D360>::type>::value));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
