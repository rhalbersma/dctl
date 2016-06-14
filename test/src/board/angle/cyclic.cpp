#include <board/group.hpp>              // axioms::is_realized, make
#include <dctl/board/angle.hpp>         // _deg, inverse, rotate
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <boost/algorithm/cxx11/all_of.hpp>                    // all_of
#include <type_traits>                  // common_type
#include <vector>                       // vector

namespace dctl {
namespace group {

BOOST_AUTO_TEST_SUITE(GroupCyclic)

BOOST_AUTO_TEST_CASE(GroupAxiomsAreRealizedOnCyclicGroups)
{
        auto const op = [](auto i, auto j){ return rotate(i, j); };
        auto const inv = [](auto i){ return inverse(i); };

        auto const C1 = make_group(
                { 0_deg },
                op, inv
        );

        auto const C2 = make_group(
                { 0_deg, 180_deg },
                op, inv
        );

        auto const C4 = make_group(
                { 0_deg,  90_deg, 180_deg, 270_deg },
                op, inv
        );

        auto const C8 = make_group(
                {   0_deg,  45_deg,  90_deg, 135_deg,
                  180_deg, 225_deg, 270_deg, 315_deg },
                op, inv
        );

        using CyclicGroup = std::common_type_t<decltype(C1), decltype(C2), decltype(C4), decltype(C8)>;

        auto const C_N = std::vector<CyclicGroup>
        {
                C1, C2, C4, C8
        };

        BOOST_CHECK(
                boost::algorithm::all_of(C_N, [](auto const& g){
                        return axioms::is_realized(g);
                })
        );
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace group
}       // namespace dctl
