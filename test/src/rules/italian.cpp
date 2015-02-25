#include <dctl/piece.hpp>               // king, pawn
#include <dctl/rules/italian.hpp>       // Rules
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_less, is_king_order, is_pawn_jump_king, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                    // adjacent_find, is_sorted
#include <cstddef>                      // size_t
#include <type_traits>                  // is_same
#include <vector>                       // vector

namespace dctl {
namespace italian {

BOOST_AUTO_TEST_SUITE(ItalianRules)

using T = Rules;

BOOST_AUTO_TEST_CASE(Traits)
{
        // required
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, short_ranged_tag>::value, "");

        // precedence
        static_assert(precedence::is_less_v<T>, "");
        static_assert(precedence::is_king_order_v<T>, "");

        // optional
        static_assert(!is_pawn_jump_king_v<T>, "");
}

BOOST_AUTO_TEST_CASE(Precedence)
{
        struct Move
        {
                std::size_t num_captured_;
                std::size_t num_captured_kings_;
                Piece with_;
                std::vector<std::size_t> king_order_;

                constexpr auto num_captured() const noexcept { return num_captured_; }
                constexpr auto num_captured(Piece) const noexcept { return num_captured_kings_; }
                constexpr auto is_with(Piece p) const noexcept { return with_ == p; }
                auto const& king_order() const noexcept { return king_order_; }
        };

        auto const moves = std::vector<Move>
        {
                { 1, 0, Piece::pawn, {      } },
                { 1, 0, Piece::king, {      } },
                { 1, 1, Piece::king, { 0    } },
                { 2, 0, Piece::pawn, {      } },
                { 2, 0, Piece::king, {      } },
                { 2, 1, Piece::king, { 0    } },
                { 2, 1, Piece::king, { 1    } },
                { 2, 2, Piece::king, { 0, 1 } },
                { 3, 0, Piece::pawn, {      } },
                { 3, 0, Piece::king, {      } },
                { 3, 1, Piece::king, { 0    } },
                { 3, 1, Piece::king, { 1    } },
                { 3, 1, Piece::king, { 2    } },
                { 3, 2, Piece::king, { 0, 1 } },
                { 3, 2, Piece::king, { 0, 2 } },
                { 3, 2, Piece::king, { 1, 2 } }
        };

        BOOST_CHECK(std::is_sorted(begin(moves), end(moves), precedence::less_t<T>{}));
        BOOST_CHECK(std::adjacent_find(begin(moves), end(moves), precedence::equal_to_t<T>{}) == end(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace italian
}       // namespace dctl
