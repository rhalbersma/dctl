#include <dctl/rule_traits.hpp>                 // is_passing_promotion
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(promotionCategory)

using VariantsStoppedpromotionTag = boost::mpl::vector
<
        checkers, czech, frisian, international, italian, pool, spanish, thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsStoppedpromotionTag, T, VariantsStoppedpromotionTag)
{
        static_assert(!is_passing_promotion_v<T>);
}

using VariantsPassingpromotionTag = boost::mpl::vector
<
        russian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsPassingpromotionTag, T, VariantsPassingpromotionTag)
{
        static_assert(is_passing_promotion_v<T>);
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
