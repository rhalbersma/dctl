#include <tuple>                        // make_tuple
#include <vector>                       // vector
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE, BOOST_CHECK
#include <dctl/angle/degrees.hpp>       // D000, D045, D090, D135, D180, D225, D270, D315
#include <dctl/angle/transform.hpp>     // inverse, rotate
#include <dctl/group/axioms.hpp>        // is_realized
#include <dctl/group/cyclic.hpp>        // Inverse, Rotate
#include <dctl/group/primitives.hpp>    // Group

namespace dctl {
namespace group {
namespace axioms {

BOOST_AUTO_TEST_SUITE(GroupAxioms)

auto make_cyclic_group(std::vector<int> const& v)
{
        return std::make_tuple(v, cyclic::Rotate(), angle::D000, cyclic::Inverse());
}

const auto C1 = make_cyclic_group(std::vector<int>{
        angle::D000,
});

const auto C2 = make_cyclic_group(std::vector<int>{
        angle::D000, angle::D180
});

const auto C4 = make_cyclic_group(std::vector<int>{
        angle::D000, angle::D090, angle::D180, angle::D270,
});

const auto C8 = make_cyclic_group(std::vector<int>{
        angle::D000, angle::D045, angle::D090, angle::D135,
        angle::D180, angle::D225, angle::D270, angle::D315
});

Group<std::vector<int>, cyclic::Rotate, int, cyclic::Inverse> const cyclic_groups[] =
{
        C1, C2, C4, C8
};

BOOST_AUTO_TEST_CASE(CyclicGroupsIsRealized)
{
        for (auto const& g: cyclic_groups) {
                //auto const first = std::begin(std::get<0>(g));
                //auto const last = std::end(std::get<0>(g));
                //for(auto e: set(g)) std::cout << e << ", "; std::cout << "\n";
                //for(auto it = first; it != last; ++it) std::cout << *it << ", "; std::cout << "\n";

                BOOST_CHECK(is_closure(g));
                BOOST_CHECK(is_associativity(g));
                BOOST_CHECK(is_identity(g));
                BOOST_CHECK(is_inverse(g));
                //BOOST_CHECK(is_realized(g));
        }
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace axioms
}       // namespace group
}       // namespace dctl
