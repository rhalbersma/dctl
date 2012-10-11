#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <type_traits>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/quote.hpp>                  // quote1
#include "../../src/angle/Degrees.hpp"
#include "../../src/angle/Traits.hpp"
#include "../../src/mpl/transform.hpp"

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(TestDegrees)

BOOST_AUTO_TEST_CASE(MirrorUp)
{
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D045, D090 >::type, D135 > ));
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D135, D090 >::type, D045 > ));
}

BOOST_AUTO_TEST_CASE(MirrorDown)
{
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D225, D270 >::type, D315 > ));
        BOOST_MPL_ASSERT(( std::is_same< mpl::mirror< D315, D270 >::type, D225 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
