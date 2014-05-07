#include <dctl/rules/variants/russian.hpp>      // Russian
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsRussian)

using T = Russian;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT((std::is_same<           range::scan<T>::type, range::distance_N >));
        BOOST_MPL_ASSERT((std::is_same< directions::pawn_jump<T>::type, directions::diag  >));
        BOOST_MPL_ASSERT((std::is_same<      precedence::jump<T>::type, precedence::none  >));

        BOOST_MPL_ASSERT((std::is_same<     promotion_phase_t<T>      , en_passant        >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
