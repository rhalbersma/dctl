#include <dctl/board/angle.hpp>         // angle, _deg
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE

using namespace dctl;

BOOST_AUTO_TEST_SUITE(AngleLiterals)

BOOST_AUTO_TEST_CASE(DegreesLiteralIsAngleObject)
{
        static_assert(  0_deg == angle{  0});
        static_assert( 45_deg == angle{ 45});
        static_assert( 90_deg == angle{ 90});
        static_assert(135_deg == angle{135});
        static_assert(180_deg == angle{180});
        static_assert(225_deg == angle{225});
        static_assert(270_deg == angle{270});
        static_assert(315_deg == angle{315});
        static_assert(360_deg == angle{360});
}

BOOST_AUTO_TEST_CASE(UnaryPlusAppliesToAngleObject)
{
        static_assert(  +0_deg == +angle{  0});
        static_assert( +45_deg == +angle{ 45});
        static_assert( +90_deg == +angle{ 90});
        static_assert(+135_deg == +angle{135});
        static_assert(+180_deg == +angle{180});
        static_assert(+225_deg == +angle{225});
        static_assert(+270_deg == +angle{270});
        static_assert(+315_deg == +angle{315});
        static_assert(+360_deg == +angle{360});
}

BOOST_AUTO_TEST_CASE(UnaryMinusAppliesToAngleObject)
{
        static_assert(  -0_deg == -angle{  0});
        static_assert( -45_deg == -angle{ 45});
        static_assert( -90_deg == -angle{ 90});
        static_assert(-135_deg == -angle{135});
        static_assert(-180_deg == -angle{180});
        static_assert(-225_deg == -angle{225});
        static_assert(-270_deg == -angle{270});
        static_assert(-315_deg == -angle{315});
        static_assert(-360_deg == -angle{360});
}

BOOST_AUTO_TEST_SUITE_END()
