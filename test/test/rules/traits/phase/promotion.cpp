#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/phase/promotion.hpp>        // promotion
#include <dctl/rules/types/phase.hpp>                   // apres_fini, en_passant
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace phase {

BOOST_AUTO_TEST_SUITE(RulesTraitsPhasePromotion)

typedef boost::mpl::vector<
        Russian
> VariantsEnPassant;

BOOST_AUTO_TEST_CASE_TEMPLATE(EnPassant, T, VariantsEnPassant)
{
        BOOST_MPL_ASSERT((std::is_same< typename promotion<T>::type, en_passant >));
}

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Spanish, Thai
> VariantsApresFini;

BOOST_AUTO_TEST_CASE_TEMPLATE(ApresFini, T, VariantsApresFini)
{
        BOOST_MPL_ASSERT((std::is_same< typename promotion<T>::type, apres_fini >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace phase
}       // namespace rules
}       // namespace dctl
