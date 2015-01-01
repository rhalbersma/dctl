#include <dctl/rule_traits/required/king_range_category.hpp>    // is_long_ranged_king_v
#include <dctl/rules.hpp>                                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE

namespace dctl {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsLongRangedKing)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, italian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsLongRangedKingV, T, VariantsFalse)
{
        static_assert(std::is_same<king_range_category_t<T>, short_ranged_tag>::value, "");
}

using VariantsTrue = boost::mpl::vector
<
        czech::Rules, frisian::Rules, international::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsLongRangedKingV, T, VariantsTrue)
{
        static_assert(std::is_same<king_range_category_t<T>, long_ranged_tag>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
