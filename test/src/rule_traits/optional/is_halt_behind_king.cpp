#include <dctl/rule_traits/optional/is_halt_behind_king.hpp>    // is_halt_behind_king
#include <dctl/rules.hpp>                                       // checkers, czech, frisian, international, italian, killer, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(IsHaltBehindKingV)

using Variants = boost::mpl::vector
<
        checkers::Rules, czech::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalseT, T, Variants)
{
        static_assert(!is_halt_behind_king_v<T>, "");
}

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrueKillerRulesT, T, Variants)
{
        static_assert(is_halt_behind_king_v<killer::Rules<T>>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
