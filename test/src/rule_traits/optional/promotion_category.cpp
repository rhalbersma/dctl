#include <dctl/rule_traits/optional/promotion_category.hpp>     // promotion_category, stopped_promotion_tag, passing_promotion_tag
#include <dctl/rules.hpp>                                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(PromotionCategoryT)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, czech::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsStoppedPromotionTag, T, VariantsFalse)
{
        static_assert(std::is_same<promotion_category_t<T>, stopped_promotion_tag>::value, "");
}

using VariantsTrue = boost::mpl::vector
<
        russian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPassingPromotionTag, T, VariantsTrue)
{
        static_assert(std::is_same<promotion_category_t<T>, passing_promotion_tag>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
