#include <dctl/rules/pool.hpp>          // Rules
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                  // is_same

namespace dctl {
namespace pool {

BOOST_AUTO_TEST_SUITE(PoolRules)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, long_ranged_tag>::value, "");

        // precedence
        static_assert(precedence::is_trivial_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace pool
}       // namespace dctl
