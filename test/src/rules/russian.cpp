#include <dctl/rules/russian.hpp>       // Rules
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_rang_category, long_ranged_tag, is_trivial, promotion_category, passing_promotion_tag
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                  // is_same

namespace dctl {
namespace russian {

BOOST_AUTO_TEST_SUITE(RussianRules)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category<T>, long_ranged_tag>::value, "");

        // precedence
        static_assert(precedence::is_trivial_v<T>, "");

        // optional
        static_assert(std::is_same<promotion_category<T>, passing_promotion_tag>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace russian
}       // namespace dctl
