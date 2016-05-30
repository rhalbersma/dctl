#include <dctl/rules/russian.hpp>       // russian
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_rang_category, long_ranged_tag, is_trivial, jumpsep, promotion_category, passing_promotion_tag
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesRussian)

using T = russian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_or_v<T>);
        static_assert(is_long_ranged_king_or_v<T>);

        static_assert(jumpsep_or_v<T> == ':');
        static_assert(is_passing_promotion_or_v<T>);

        static_assert(is_trivial_precedence_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
