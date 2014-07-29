#include <dctl/rule_traits.hpp>                 // is_pawn_jump_king_v
#include <dctl/rules.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsIsPawnJumpKing)

using VariantsTrue = boost::mpl::vector<
        Checkers, Czech, Frisian, International, Killer, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPawnJumpKingV, T, VariantsTrue)
{
        static_assert(is_pawn_jump_king_v<T>, "");
}

using VariantsFalse = boost::mpl::vector<
        Italian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsPawnJumpKingV, T, VariantsFalse)
{
        static_assert(!is_pawn_jump_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
