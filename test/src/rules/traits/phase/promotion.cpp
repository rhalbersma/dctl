#include <dctl/rules/traits/is_en_passant_promotion.hpp>        // promotion
#include <dctl/rules/variants.hpp>
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                                  // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsPhasePromotion)

using VariantsEnPassant = boost::mpl::vector<
        Russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(PromotionPhaseIsEnPassant, Rules, VariantsEnPassant)
{
        BOOST_MPL_ASSERT((std::is_same<is_en_passant_promotion_t<Rules>, std::true_type>));
}

using VariantsApresFini = boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(PromotionPhaseIsApresFini, Rules, VariantsApresFini)
{
        BOOST_MPL_ASSERT((std::is_same<is_en_passant_promotion_t<Rules>, std::false_type>));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
