#include <type_traits>                  // is_same
#include <boost/mpl/assert.hpp>         // BOOST_MPL_ASSERT
#include <boost/test/unit_test.hpp>     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <dctl/rules/types.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(TestItalian)

typedef rules::Italian T;

BOOST_AUTO_TEST_CASE(Traits)
{
        BOOST_MPL_ASSERT((std::is_same<                  range::scan<T>::type, range::distance_1   >));
        BOOST_MPL_ASSERT((std::is_same<        directions::pawn_jump<T>::type, directions::up      >));
        BOOST_MPL_ASSERT((std::is_same<             precedence::jump<T>::type, precedence::quality >));

        BOOST_MPL_ASSERT((std::is_same<          is_pawns_jump_kings<T>::type, std::false_type     >));
        BOOST_MPL_ASSERT((std::is_same< precedence::is_relative_king<T>::type, std::true_type      >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
