#include <algorithm>                            // all_of
#include <iterator>                             // begin, end
#include <boost/iterator/counting_iterator.hpp> // counting_iterator
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>               // D000, D045, D090, D135, D180, D225, D270, D315, D360
#include <dctl/group/action.hpp>                // is_realized
#include <dctl/group/axioms.hpp>                // is_realized
#include <dctl/group/cyclic.hpp>                // CyclicGroup, make_cyclic_group

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

BOOST_AUTO_TEST_CASE(AllGroupAxiomsIsRealized)
{
        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [](CyclicGroup const& g){
                        return axioms::is_realized(g);
                })
        );
}

BOOST_AUTO_TEST_CASE(AllGroupActionIsRealizedOnAllAngles)
{
        auto const alpha = boost::counting_iterator<int>(angle::D000);
        auto const omega = boost::counting_iterator<int>(angle::D360);

        BOOST_CHECK(
                std::all_of(std::begin(C_N), std::end(C_N), [=](CyclicGroup const& g){
                        return std::all_of(alpha, omega, [&](int const& i){
                                return action::is_realized(i, g);
                        });
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace cyclic
}       // namespace group
}       // namespace dctl
