#include <dctl/rules/variants/thai.hpp> // Thai
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                  // is_same, true_type

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsThai)

using T = Thai;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT(( is_long_ranged_king_t<T> ));
        BOOST_MPL_ASSERT((std::is_same<   directions::pawn_jump<T>::type, directions::up                 >));
        BOOST_MPL_ASSERT((std::is_same<        precedence::jump<T>::type, precedence::none               >));

        BOOST_MPL_ASSERT((std::is_same<             range::land<T>::type, range::distance_1              >));
        BOOST_MPL_ASSERT(( is_reversible_king_jump_direction_t<T>                                           ));
        BOOST_MPL_ASSERT((std::is_same<  is_en_passant_jump_removal_t<T>, std::true_type                 >));
        BOOST_MPL_ASSERT((std::is_same<             initial_gap<T>::type, std::integral_constant<int, 4> >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
