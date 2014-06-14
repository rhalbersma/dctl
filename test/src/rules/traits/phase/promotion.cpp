#include <dctl/rules/traits/is_en_passant_promotion.hpp>        // promotion
#include <dctl/rules/variants.hpp>
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsPhasePromotion)

using VariantsEnPassant = boost::mpl::vector<
        Russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(PromotionPhaseIsEnPassant, Rules, VariantsEnPassant)
{
        static_assert(is_en_passant_promotion_v<Rules>, "");
}

using VariantsApresFini = boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(PromotionPhaseIsApresFini, Rules, VariantsApresFini)
{
        static_assert(!is_en_passant_promotion_v<Rules>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
