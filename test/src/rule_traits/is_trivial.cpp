#include <dctl/rule_traits.hpp>                 // is_trivial
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace precedence {

BOOST_AUTO_TEST_SUITE(PrecedenceIsTrivial)

using VariantsTrivial = boost::mpl::vector
<
        checkers::Rules, pool::Rules, russian::Rules, thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrivial, T, VariantsTrivial)
{
        static_assert(is_trivial_v<T>, "");
}

using VariantsNonTrivial = boost::mpl::vector
<
        czech::Rules, frisian::Rules, international::Rules, italian::Rules, spanish::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsNonTrivial, T, VariantsNonTrivial)
{
        static_assert(!is_trivial_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace precedence
}       // namespace dctl
