#include <dctl/piece.hpp>               // king, pawn
#include <dctl/rules/czech.hpp>         // Rules
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, is_absolute_with_king, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                    // adjacent_find, is_sorted
#include <cstddef>                      // size_t
#include <type_traits>                  // is_same
#include <vector>                       // vector

namespace dctl {
namespace czech {

BOOST_AUTO_TEST_SUITE(CzechRules)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category<T>, long_ranged_tag>::value, "");

        // precedence
        static_assert(!precedence::is_trivial_v<T>, "");
        static_assert(precedence::is_absolute_with_king_v<T>, "");
}

BOOST_AUTO_TEST_CASE(Precedence)
{
        struct Move
        {
                Piece with;
                constexpr auto is_with(Piece p) const noexcept { return with == p; }
        };

        auto const moves = std::vector<Move>
        {
                { Piece::pawn },
                { Piece::king }
        };

        BOOST_CHECK(std::is_sorted(begin(moves), end(moves), precedence::less<T>{}));
        BOOST_CHECK(std::adjacent_find(begin(moves), end(moves), precedence::equal_to<T>{}) == end(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace czech
}       // namespace dctl
