#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, killer, pool, russian, spanish, thai
#include <dctl/rule_traits.hpp>                 // is_halt_behind_king
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <type_traits>                          // is_base_of

namespace dctl {
namespace killer {

BOOST_AUTO_TEST_SUITE(KillerRules)

using Variants = boost::mpl::vector
<
        checkers::Rules, czech::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Traits, T, Variants)
{
        static_assert(is_halt_behind_king_v<killer::Rules<T>>, "");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsBaseOf, T, Variants)
{
        static_assert(std::is_base_of<T, killer::Rules<T>>::value, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace killer
}       // namespace dctl
