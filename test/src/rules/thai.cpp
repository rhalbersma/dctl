#include <dctl/rules/thai.hpp>          // Rules
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, initial_gap, is_land_behind_piece, is_en_passant_jump_removal
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                  // is_same

namespace dctl {
namespace thai {

BOOST_AUTO_TEST_SUITE(ThaiRules)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category<T>, long_ranged_tag>::value, "");

        // precedence
        static_assert(precedence::is_trivial_v<T>, "");

        // optional
        static_assert(initial_gap_v<T> == 4, "");
        static_assert(is_land_behind_piece_v<T>, "");
        static_assert(is_en_passant_jump_removal_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace thai
}       // namespace dctl
