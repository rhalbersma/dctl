#include <dctl/rules/czech.hpp>         // Czech
#include <rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/color_piece.hpp>         // king, pawn
#include <dctl/rule_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial, jumpsep, is_absolute_modality, equal_to, less
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

        static_assert(is_nontrivial_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);

        struct Action
        {
                using rules_type = Czech;
                piece with;
                constexpr auto is_with_king() const noexcept { return with == piece::king; }
        };

        auto const moves = std::vector<Action>
        {
                { piece::pawn },
                { piece::king }
        };

        BOOST_CHECK(precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
