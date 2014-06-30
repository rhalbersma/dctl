#include <dctl/rules/variants/thai.hpp>         // Thai
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsThai)

using T = Thai;

BOOST_AUTO_TEST_CASE(Traits)
{
        static_assert(is_long_ranged_king_v<T>, "");
        static_assert(!is_backward_pawn_jump_v<T>, "");
        static_assert(!is_jump_precedence_v<T>, "");

        BOOST_MPL_ASSERT((std::is_same<range::land<T>::type, range::distance_1>));
        static_assert(is_en_passant_jump_removal_v<T>, "");
        static_assert(initial_gap_v<T> == 4, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
