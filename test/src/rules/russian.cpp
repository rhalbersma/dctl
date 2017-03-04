#include <dctl/rules/russian.hpp>       // russian
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, is_long_ranged_king, jumpsep, is_passing_promotion, is_trivial_precedence
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(Rulesrussian)

using T = russian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(jumpsep_v<T> == ':');
        static_assert(is_passing_promotion_v<T>);

        static_assert(is_trivial_precedence_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
