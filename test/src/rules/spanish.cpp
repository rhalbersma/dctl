#include <dctl/rules/spanish.hpp>       // Spanish
#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/piece.hpp>               // Piece
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, is_long_ranged_king, is_trivial, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <cstddef>                      // size_t
#include <vector>                       // vector

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesSpanish)

using T = Spanish;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_or_v<T>);
        static_assert(is_long_ranged_king_or_v<T>);

        static_assert(is_nontrivial_precedence_v<T>);
        static_assert(is_quantity_precedence_or_v<T>);
        static_assert(is_quality_precedence_or_v<T>);

        struct Action
        {
                using rules_type = Spanish;
                std::size_t num_captured_;
                std::size_t num_captured_kings_;
                constexpr auto num_captured_pieces()       const noexcept { return num_captured_;       }
                constexpr auto num_captured_kings() const noexcept { return num_captured_kings_; }
        };

        auto const moves = std::vector<Action>
        {
                { 1, 0 },
                { 1, 1 },
                { 2, 0 },
                { 2, 1 },
                { 2, 2 }
        };

        BOOST_CHECK(precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
