#include <dctl/rule_traits/required/king_range_category.hpp>    // king_range_category, long_ranged_tag, short_ranged_tag
#include <dctl/rules.hpp>                                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                                          // is_same

namespace dctl {

BOOST_AUTO_TEST_SUITE(KingRangeCategoryT)

using VariantsShort = boost::mpl::vector
<
        checkers::Rules, italian::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSameAsShortRangedTag, T, VariantsShort)
{
        static_assert(std::is_same<king_range_category<T>, short_ranged_tag>::value, "");
}

using VariantsLong = boost::mpl::vector
<
        czech::Rules, frisian::Rules, international::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsSameAsLongRangedTag, T, VariantsLong)
{
        static_assert(std::is_same<king_range_category<T>, long_ranged_tag>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace dctl
