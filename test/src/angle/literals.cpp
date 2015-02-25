#include <dctl/angle.hpp>               // Angle, _deg
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE

using namespace dctl;

BOOST_AUTO_TEST_SUITE(AngleLiterals)

BOOST_AUTO_TEST_CASE(DegreesLiteralIsAngleObject)
{
        static_assert(  0_deg == Angle{  0}, "");
        static_assert( 45_deg == Angle{ 45}, "");
        static_assert( 90_deg == Angle{ 90}, "");
        static_assert(135_deg == Angle{135}, "");
        static_assert(180_deg == Angle{180}, "");
        static_assert(225_deg == Angle{225}, "");
        static_assert(270_deg == Angle{270}, "");
        static_assert(315_deg == Angle{315}, "");
        static_assert(360_deg == Angle{360}, "");
}

BOOST_AUTO_TEST_CASE(UnaryPlusAppliesToAngleObject)
{
        static_assert(  +0_deg == +Angle{  0}, "");
        static_assert( +45_deg == +Angle{ 45}, "");
        static_assert( +90_deg == +Angle{ 90}, "");
        static_assert(+135_deg == +Angle{135}, "");
        static_assert(+180_deg == +Angle{180}, "");
        static_assert(+225_deg == +Angle{225}, "");
        static_assert(+270_deg == +Angle{270}, "");
        static_assert(+315_deg == +Angle{315}, "");
        static_assert(+360_deg == +Angle{360}, "");
}

BOOST_AUTO_TEST_CASE(UnaryMinusAppliesToAngleObject)
{
        static_assert(  -0_deg == -Angle{  0}, "");
        static_assert( -45_deg == -Angle{ 45}, "");
        static_assert( -90_deg == -Angle{ 90}, "");
        static_assert(-135_deg == -Angle{135}, "");
        static_assert(-180_deg == -Angle{180}, "");
        static_assert(-225_deg == -Angle{225}, "");
        static_assert(-270_deg == -Angle{270}, "");
        static_assert(-315_deg == -Angle{315}, "");
        static_assert(-360_deg == -Angle{360}, "");
}

BOOST_AUTO_TEST_SUITE_END()
