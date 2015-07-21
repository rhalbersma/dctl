#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_rang_category, long_ranged_tag, is_trivial, jumpsep, promotion_category, passing_promotion_tag
#include <dctl/rules.hpp>               // Russian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                  // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesRussian)

using T = Russian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, long_ranged_tag>::value, "");

        static_assert(jumpsep_v<T> == ':', "");
        static_assert(std::is_same<promotion_category_t<T>, passing_promotion_tag>::value, "");

        static_assert(precedence::is_complete_v<T>, "");
        static_assert(precedence::is_trivial_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
