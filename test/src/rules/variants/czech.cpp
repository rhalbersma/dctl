#include <dctl/rules/variants/czech.hpp>        // Czech
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsCzech)

using T = Czech;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT(( is_long_ranged_king_t<T> ));
        BOOST_MPL_ASSERT((std::is_same< directions::pawn_jump<T>::type, directions::up    >));
        BOOST_MPL_ASSERT((std::is_same<      precedence::jump<T>::type, precedence::none  >));

        BOOST_MPL_ASSERT(( is_absolute_king_jump_precedence_t<T>                           ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
