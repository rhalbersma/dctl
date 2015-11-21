#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, short_ranged_tag, is_trivial
#include <dctl/rules.hpp>               // Checkers
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <experimental/type_traits>     // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesCheckers)

using T = Checkers;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(std::experimental::is_same_v<king_range_category_t<T>, short_ranged_tag>);

        static_assert(precedence::is_trivial_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
