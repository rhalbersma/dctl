#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, initial_gap, is_land_behind_piece, capture_category, passing_capture_tag, jumpsep
#include <dctl/rules.hpp>               // Thai
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <experimental/type_traits>     // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesThai)

using T = Thai;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(std::experimental::is_same_v<king_range_category_t<T>, long_ranged_tag>);

        static_assert(initial_position_gap_v<T> == 4);
        static_assert(is_land_behind_piece_v<T>);
        static_assert(std::experimental::is_same_v<capture_category_t<T>, passing_capture_tag>);
        static_assert(jumpsep_v<T> == '-');

        static_assert(precedence::is_trivial_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
