#include <type_traits>                  // is_same
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/mpl/placeholders.hpp>   // _1
#include <boost/mpl/quote.hpp>          // quote1
#include <boost/mpl/vector.hpp>         // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/angle/Degrees.hpp>       // D045, D090, D135, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse, rotate, mirror
#include <dctl/mpl/type_traits.hpp>     // is_idempotent

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(TestTransform)

typedef boost::mpl::vector<
        D000, D045, D090, D135,
        D180, D225, D270, D315
> DegreesSequence;

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentInverse, T, DegreesSequence)
{
        BOOST_MPL_ASSERT(( mpl::is_idempotent< boost::mpl::quote1< mpl::inverse >, T > ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentRotate180, T, DegreesSequence)
{
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::rotate< boost::mpl::_1, D180 >, T > ));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IdemPotentMirror, T, DegreesSequence)
{
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D000 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D045 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D090 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D135 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D180 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D225 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D270 >, T > ));
        BOOST_MPL_ASSERT(( mpl::is_idempotent< mpl::mirror< boost::mpl::_1, D315 >, T > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
