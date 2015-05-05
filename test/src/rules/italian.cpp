#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/piece.hpp>               // king, pawn
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, is_pawn_jump_king, is_king_order, equal_to, less
#include <dctl/rules.hpp>               // Italian
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <algorithm>                    // adjacent_find, is_sorted
#include <cstddef>                      // size_t
#include <type_traits>                  // is_same
#include <vector>                       // vector

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesItalian)

using T = Italian;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(std::is_same<king_range_category_t<T>, short_ranged_tag>::value, "");
        static_assert(!precedence::is_trivial_v<T>, "");

        static_assert(!is_pawn_jump_king_v<T>, "");
        static_assert(precedence::is_king_order_v<T>, "");

        struct Move
        {
                std::size_t num_captured_;
                std::size_t num_captured_kings_;
                Piece with_;
                std::vector<std::size_t> king_order_;

                constexpr auto num_captured()      const noexcept { return num_captured_;       }
                constexpr auto num_captured(Piece) const noexcept { return num_captured_kings_; }
                constexpr auto is_with(Piece p)    const noexcept { return with_ == p;          }
                auto const& king_order()           const noexcept { return king_order_;         }
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

        BOOST_CHECK(precedence::is_consistent<T>(begin(moves), end(moves)));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
