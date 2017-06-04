#include <dctl/core/board/angle.hpp>         // angle, _deg
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE

using namespace dctl::core;
using namespace board::literals;

BOOST_AUTO_TEST_SUITE(angleLiterals)

BOOST_AUTO_TEST_CASE(DegreesLiteralIsangleObject)
{
        static_assert(  0_deg == board::angle{  0});
        static_assert( 45_deg == board::angle{ 45});
        static_assert( 90_deg == board::angle{ 90});
        static_assert(135_deg == board::angle{135});
        static_assert(180_deg == board::angle{180});
        static_assert(225_deg == board::angle{225});
        static_assert(270_deg == board::angle{270});
        static_assert(315_deg == board::angle{315});
        static_assert(360_deg == board::angle{360});
}

BOOST_AUTO_TEST_CASE(UnaryPlusAppliesToangleObject)
{
        static_assert(  +0_deg == +board::angle{  0});
        static_assert( +45_deg == +board::angle{ 45});
        static_assert( +90_deg == +board::angle{ 90});
        static_assert(+135_deg == +board::angle{135});
        static_assert(+180_deg == +board::angle{180});
        static_assert(+225_deg == +board::angle{225});
        static_assert(+270_deg == +board::angle{270});
        static_assert(+315_deg == +board::angle{315});
        static_assert(+360_deg == +board::angle{360});
}

BOOST_AUTO_TEST_CASE(UnaryMinusAppliesToangleObject)
{
        static_assert(  -0_deg == -board::angle{  0});
        static_assert( -45_deg == -board::angle{ 45});
        static_assert( -90_deg == -board::angle{ 90});
        static_assert(-135_deg == -board::angle{135});
        static_assert(-180_deg == -board::angle{180});
        static_assert(-225_deg == -board::angle{225});
        static_assert(-270_deg == -board::angle{270});
        static_assert(-315_deg == -board::angle{315});
        static_assert(-360_deg == -board::angle{360});
}

BOOST_AUTO_TEST_SUITE_END()
