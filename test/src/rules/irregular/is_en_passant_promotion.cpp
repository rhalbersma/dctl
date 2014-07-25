#include <dctl/rules/irregular/is_en_passant_promotion.hpp>     // is_en_passant_promotion_v
#include <dctl/rules/variants.hpp>
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsIsEnPassantPromotion)

using VariantsFalse = boost::mpl::vector<
        Checkers, Czech, Frisian, International, Italian, Killer, Pool, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsEnPassantPromotionV, T, VariantsFalse)
{
        static_assert(!is_en_passant_promotion_v<T>, "");
}

using VariantsTrue = boost::mpl::vector<
        Russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsEnPassantPromotionV, T, VariantsTrue)
{
        static_assert(is_en_passant_promotion_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
