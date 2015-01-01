#include <dctl/rules/thai.hpp>          // Thai
#include <dctl/rule_traits.hpp>         // is_backward_pawn, is_jump_precedence, is_long_ranged_king,
                                        // initial_gap, is_directly_land_after_piece, is_en_passant_jump_removal
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace thai {

BOOST_AUTO_TEST_SUITE(RulesVariantsThai)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(!is_jump_precedence_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, long_ranged_tag>::value, "");

        // optional
        static_assert(initial_gap_v<T> == 4, "");
        static_assert(is_directly_land_after_piece_v<T>, "");
        static_assert(is_en_passant_jump_removal_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace thai
}       // namespace dctl
