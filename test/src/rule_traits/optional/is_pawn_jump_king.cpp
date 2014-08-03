#include <dctl/rule_traits/optional/is_pawn_jump_king.hpp>      // is_pawn_jump_king
#include <dctl/rules.hpp>                                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsPawnJumpKing)

using VariantsTrue = boost::mpl::vector
<
        Checkers, Czech, Frisian, International, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPawnJumpKingV, T, VariantsTrue)
{
        static_assert(is_pawn_jump_king_v<T>, "");
}

using VariantsFalse = boost::mpl::vector
<
        Italian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsPawnJumpKingV, T, VariantsFalse)
{
        static_assert(!is_pawn_jump_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
