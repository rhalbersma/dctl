#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // C1, C2, C4, C8

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleDegrees)

using Octants = boost::mpl::vector
<
        angle::Degrees<angle::D000>, angle::Degrees<angle::D045>,
        angle::Degrees<angle::D090>, angle::Degrees<angle::D135>,
        angle::Degrees<angle::D180>, angle::Degrees<angle::D225>,
        angle::Degrees<angle::D270>, angle::Degrees<angle::D315>
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(RightAction, T, Octants)
{
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C1 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C2 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C4 > ));
        BOOST_MPL_ASSERT(( group::action::is_realized< T, group::C8 > ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace angle
}       // namespace dctl
