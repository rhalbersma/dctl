#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/directions/pawn_turn.hpp>   // pawn_jump
#include <dctl/rules/types/directions.hpp>              // up, diag, all
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace directions {

BOOST_AUTO_TEST_SUITE(RulesTraitsDirectionsPawnTurn)

typedef boost::mpl::vector<
        Checkers, Czech, Italian, Spanish, Thai
> VariantsUp;

BOOST_AUTO_TEST_CASE_TEMPLATE(PawnTurnUp, T, VariantsUp)
{
        BOOST_MPL_ASSERT((std::is_same< typename pawn_jump<T>::type, up >));
}

typedef boost::mpl::vector<
        AKWDamme, HHoogland, International, Killer, Pool, Russian
> VariantsDiag;

BOOST_AUTO_TEST_CASE_TEMPLATE(PawnTurnDiag, T, VariantsDiag)
{
        BOOST_MPL_ASSERT((std::is_same< typename pawn_jump<T>::type, diag >));
}

typedef boost::mpl::vector<
        Frisian
> VariantsAll;

BOOST_AUTO_TEST_CASE_TEMPLATE(PawnTurnAll, T, VariantsAll)
{
        BOOST_MPL_ASSERT((std::is_same< typename pawn_jump<T>::type, all >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace directions
}       // namespace rules
}       // namespace dctl
