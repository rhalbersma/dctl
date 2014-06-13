#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/orthogonality/king_move.hpp>        // king_jump
#include <dctl/rules/types/orthogonality.hpp>                   // none /*, relative */
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace orthogonality {

BOOST_AUTO_TEST_SUITE(RulesTraitsOrthogonalityKingMove)

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish, Thai
> VariantsNone;

BOOST_AUTO_TEST_CASE_TEMPLATE(KingMoveNone, T, VariantsNone)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_move<T>::type, none >));
}

/*
typedef boost::mpl::vector<
        Lineo
> VariantsRelative;

BOOST_AUTO_TEST_CASE_TEMPLATE(KingJumpRelative, T, VariantsAll)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_jump<T>::type, relative >));
}
*/

BOOST_AUTO_TEST_SUITE_END()

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
