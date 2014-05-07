#include <dctl/rules/variants/thai.hpp> // Thai
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                  // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsThai)

typedef Thai T;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT((std::is_same<             range::scan<T>::type, range::distance_N              >));
        BOOST_MPL_ASSERT((std::is_same<   directions::pawn_jump<T>::type, directions::up                 >));
        BOOST_MPL_ASSERT((std::is_same<        precedence::jump<T>::type, precedence::none               >));

        BOOST_MPL_ASSERT((std::is_same<             range::land<T>::type, range::distance_1              >));
        BOOST_MPL_ASSERT((              directions::is_reversal<T>                                        ));
        BOOST_MPL_ASSERT((std::is_same<         jump_removal_phase_t<T>,       en_passant                     >));
        BOOST_MPL_ASSERT((std::is_same<             initial_gap<T>::type, std::integral_constant<int, 4> >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
