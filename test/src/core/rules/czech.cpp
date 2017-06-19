#include <core/rules/precedence.hpp>         // precedence::is_consistent
#include <dctl/core/rules/czech.hpp>         // czech
#include <dctl/core/state/color_piece.hpp>         // king, pawn
#include <dctl/core/rules/type_traits.hpp>         // is_backward_pawn_jump, king_range_category, long_ranged_tag, is_trivial_precedence, jumpsep, is_absolute_modality, equal_to, less
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <vector>                       // vector

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(Rulesczech)

using T = czech;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(not is_backward_pawn_jump_v<T>);
        static_assert(is_long_ranged_king_v<T>);

        static_assert(jumpsep_v<T> == ':');

        static_assert(not is_trivial_precedence_v<T>);
        static_assert(is_modality_precedence_v<T>);

        struct Action
        {
                using rules_type [[maybe_unused]] = czech;
                piece with;
                constexpr auto is_with_king() const noexcept { return with == piece::kings; }
        };

        auto const moves = std::vector<Action>
        {
                { piece::pawns },
                { piece::kings }
        };

        BOOST_CHECK(precedence::is_consistent(moves));
}

BOOST_AUTO_TEST_SUITE_END()
