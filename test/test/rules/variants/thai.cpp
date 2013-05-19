#include <type_traits>
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/enum.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(TestThai)

typedef rules::Thai T;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT((std::is_same<                 king_range<T>::type, range::distance_N              >));
        BOOST_MPL_ASSERT((std::is_same<       pawn_jump_directions<T>::type, directions::up                 >));
        BOOST_MPL_ASSERT((std::is_same<            jump_precedence<T>::type, precedence::none               >));

        BOOST_MPL_ASSERT((std::is_same<                 land_range<T>::type, range::distance_1              >));
        BOOST_MPL_ASSERT((std::is_same< is_jump_direction_reversal<T>::type, std::true_type                 >));
        BOOST_MPL_ASSERT((std::is_same<           captures_removal<T>::type, removal::en_passant            >));
        BOOST_MPL_ASSERT((std::is_same<                initial_gap<T>::type, std::integral_constant<int, 4> >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
