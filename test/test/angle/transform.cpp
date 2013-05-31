#include <type_traits>                  // is_same
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/placeholders.hpp>   // _1
#include <boost/mpl/quote.hpp>          // quote1
#include <boost/mpl/vector.hpp>         // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/angle/degrees.hpp>       // D045, D090, D135, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse, rotate, mirror
#include <dctl/mpl/type_traits.hpp>     // is_idempotent

namespace dctl {
namespace mpl {
namespace lazy {

BOOST_AUTO_TEST_SUITE(AngleTransform)

typedef boost::mpl::vector<
        angle::D000, angle::D045, angle::D090, angle::D135,
        angle::D180, angle::D225, angle::D270, angle::D315
> DegreesSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, DegreesSequence)
{
        BOOST_MPL_ASSERT(( is_idempotent< boost::mpl::quote1< inverse >, T > ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentRotate180, T, DegreesSequence)
{
        BOOST_MPL_ASSERT(( is_idempotent< rotate< boost::mpl::_1, angle::D180 >, T > ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentMirror, T, DegreesSequence)
{
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D000 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D045 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D090 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D135 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D180 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D225 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D270 >, T > ));
        BOOST_MPL_ASSERT(( is_idempotent< mirror< boost::mpl::_1, angle::D315 >, T > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace lazy
}       // namespace mpl
}       // namespace dctl
