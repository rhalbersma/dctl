#include <type_traits>                  // is_same
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/enum.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(TestPool)

typedef rules::Pool T;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT((std::is_same<           king_range<T>::type, range::distance_N >));
        BOOST_MPL_ASSERT((std::is_same< pawn_jump_directions<T>::type, directions::diag  >));
        BOOST_MPL_ASSERT((std::is_same<      jump_precedence<T>::type, precedence::none  >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
