#include <dctl/rules/italian.hpp>      // Italian
#include <dctl/rule_traits.hpp>
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsItalian)

using T = Italian;

BOOST_AUTO_TEST_CASE(Traits)
{
        static_assert(!is_long_ranged_king_v<T>, "");
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(is_jump_precedence_v<T>, "");

        static_assert(!is_pawn_jump_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
