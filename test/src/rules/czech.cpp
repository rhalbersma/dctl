#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/piece.hpp>               // king, pawn
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, jumpsep, is_absolute_modality, equal_to, less
#include <dctl/rules.hpp>               // Czech
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesCzech)

using T = Czech;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(!is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(jumpsep_v<T> == ':');

        static_assert(!is_trivial_precedence_v<T>);
        static_assert( is_modality_precedence_v<T>);

        struct Action
        {
                Piece with;
                constexpr auto is_with_king() const noexcept { return with == Piece::king; }
        };

        auto const moves = std::vector<Action>
        {
                { Piece::pawn },
                { Piece::king }
        };

        BOOST_CHECK(precedence::is_consistent<T>(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
