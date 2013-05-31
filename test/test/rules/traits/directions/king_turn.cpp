#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/directions/king_turn.hpp>   // king_jump
#include <dctl/rules/types/directions.hpp>              // diag, all
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace directions {

BOOST_AUTO_TEST_SUITE(RulesTraitsDirectionsKingTurn)

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, International, Italian, Killer, Pool, Russian, Spanish, Thai
> VariantsDiag;

BOOST_AUTO_TEST_CASE_TEMPLATE(Diag, T, VariantsDiag)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_jump<T>::type, diag >));
}

typedef boost::mpl::vector<
        Frisian, HHoogland
> VariantsAll;

BOOST_AUTO_TEST_CASE_TEMPLATE(All, T, VariantsAll)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_jump<T>::type, all >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace directions
}       // namespace rules
}       // namespace dctl
