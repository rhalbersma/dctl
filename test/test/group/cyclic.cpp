#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/angle.hpp>         // make_angle
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/group/axioms.hpp>        // is_realized
#include <dctl/group/action.hpp>        // is_realized
#include <dctl/group/cyclic.hpp>        // CyclicGroup, make_cyclic_group

namespace dctl {
namespace group {
namespace cyclic {

BOOST_AUTO_TEST_SUITE(GroupCyclic)

auto const C1 = make_cyclic_group({
        angle::D000,
});

auto const C2 = make_cyclic_group({
        angle::D000, angle::D180
});

auto const C4 = make_cyclic_group({
        angle::D000, angle::D090, angle::D180, angle::D270,
});

auto const C8 = make_cyclic_group({
        angle::D000, angle::D045, angle::D090, angle::D135,
        angle::D180, angle::D225, angle::D270, angle::D315
});

CyclicGroup const C_N[] =
{
        C1, C2, C4, C8
};

BOOST_AUTO_TEST_CASE(AxiomsIsRealized)
{
        for (auto const& g: C_N)
                BOOST_CHECK(axioms::is_realized(g));
}

BOOST_AUTO_TEST_CASE(ActionIsRealizedOnAllAngles)
{
        for (auto const& g: C_N)
                for (auto i = 0; i != 360; ++i)
                        BOOST_CHECK(action::is_realized(make_angle(i), g));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace cyclic
}       // namespace group
}       // namespace dctl
