#include <dctl/rule_traits.hpp>                 // is_long_ranged_king_v
#include <dctl/rules.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                          // is_same

namespace dctl {
namespace rules {
namespace range {

BOOST_AUTO_TEST_SUITE(RulesTraitsRangeScan)

using VariantsFalse = boost::mpl::vector<
        Checkers, Italian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsLongRangedKingV, T, VariantsFalse)
{
        static_assert(!is_long_ranged_king_v<T>, "");
}

using VariantsTrue = boost::mpl::vector<
        Czech, Frisian, International, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsLongRangedKingV, T, VariantsTrue)
{
        static_assert(is_long_ranged_king_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace range
}       // namespace rules
}       // namespace dctl
