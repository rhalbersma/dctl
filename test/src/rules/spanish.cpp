#include <dctl/piece.hpp>               // Piece
#include <dctl/rules/spanish.hpp>       // Rules
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                    // adjacent_find, is_sorted
#include <cstddef>                      // size_t
#include <type_traits>                  // is_same
#include <vector>                       // vector

namespace dctl {
namespace spanish {

BOOST_AUTO_TEST_SUITE(SpanishRules)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category<T>, long_ranged_tag>::value, "");

        // precedence
        static_assert(!precedence::is_trivial_v<T>, "");
}

BOOST_AUTO_TEST_CASE(Precedence)
{
        struct Move
        {
                std::size_t num_captured_;
                std::size_t num_captured_kings_;
                constexpr auto num_captured() const noexcept { return num_captured_; }
                constexpr auto num_captured(Piece) const noexcept { return num_captured_kings_; }
        };

        auto const moves = std::vector<Move>
        {
                { 1, 0 },
                { 1, 1 },
                { 2, 0 },
                { 2, 1 },
                { 2, 2 }
        };

        BOOST_CHECK(std::is_sorted(begin(moves), end(moves), precedence::less<T>{}));
        BOOST_CHECK(std::adjacent_find(begin(moves), end(moves), precedence::equal_to<T>{}) == end(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace spanish
}       // namespace dctl
