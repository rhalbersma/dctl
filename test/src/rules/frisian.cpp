#include <dctl/rules/frisian.hpp>       // Frisian
#include <dctl/rule_traits.hpp>         // is_backward_pawn, is_jump_precedence, is_long_ranged_king, is_orthogonal_jump
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace frisian {

BOOST_AUTO_TEST_SUITE(RulesVariantsFrisian)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(is_backward_pawn_jump_v<T>, "");
        static_assert(is_jump_precedence_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, long_ranged_tag>::value, "");

        // optional
        static_assert(is_orthogonal_jump_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace frisian
}       // namespace dctl
