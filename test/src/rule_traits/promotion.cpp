#include <dctl/rule_traits.hpp>                 // promotion_category, stopped_promotion_tag, passing_promotion_tag
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(PromotionCategory)

using VariantsStoppedPromotionTag = boost::mpl::vector
<
        Checkers, Czech, Frisian, International, Italian, Pool, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsStoppedPromotionTag, T, VariantsStoppedPromotionTag)
{
        static_assert(std::is_same<promotion_category_t<T>, stopped_promotion_tag>::value, "");
}

using VariantsPassingPromotionTag = boost::mpl::vector
<
        Russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPassingPromotionTag, T, VariantsPassingPromotionTag)
{
        static_assert(std::is_same<promotion_category_t<T>, passing_promotion_tag>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
