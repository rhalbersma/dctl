#include <type_traits>                  // is_same
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/angle/Degrees.hpp>       // D045, D090, D135, D225, D270, D315
#include <dctl/mpl/transform.hpp>       // mirror

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(TestDegrees)

BOOST_AUTO_TEST_CASE(testMirrorUp)
{
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D045, D090 >::type, D135 > ));
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D135, D090 >::type, D045 > ));
}

BOOST_AUTO_TEST_CASE(testMirrorDown)
{
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D225, D270 >::type, D315 > ));
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D315, D270 >::type, D225 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
