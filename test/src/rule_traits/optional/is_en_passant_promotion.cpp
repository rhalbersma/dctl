#include <dctl/rule_traits/optional/is_en_passant_promotion.hpp>        // is_en_passant_promotion
#include <dctl/rules.hpp>                                               // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                         // vector
#include <boost/test/test_case_template.hpp>                            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsEnPassantPromotion)

using VariantsFalse = boost::mpl::vector
<
        Checkers, Czech, Frisian, International, Italian, Pool, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsEnPassantPromotionV, T, VariantsFalse)
{
        static_assert(!is_en_passant_promotion_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        Russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsEnPassantPromotionV, T, VariantsTrue)
{
        static_assert(is_en_passant_promotion_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
