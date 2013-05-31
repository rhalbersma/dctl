#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // C1, C2, C4, C8

namespace dctl {
namespace angle {

BOOST_AUTO_TEST_SUITE(AngleDegrees)

typedef boost::mpl::vector<
        D000, D045, D090, D135,
        D180, D225, D270, D315
> Octants;

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
