#include <dctl/core/rules/checkers.hpp>         // checkers
#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, is_long_ranged_king, is_trivial_precedence
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(Rulescheckers)

using T = checkers;

BOOST_AUTO_TEST_CASE(RuleTraits)
{
        static_assert(not is_backward_pawn_jump_v<T>);
        static_assert(not is_long_ranged_king_v<T>);
        static_assert(is_trivial_precedence_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()
