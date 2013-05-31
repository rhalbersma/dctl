#include <type_traits>                          // is_same, integral_constant
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/large_jump.hpp>     // large_jump
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsLargeJump)

typedef boost::mpl::vector<
        Frisian, Thai
> Variants3;

BOOST_AUTO_TEST_CASE_TEMPLATE(LargeJump3, T, Variants3)
{
        BOOST_MPL_ASSERT((std::is_same< typename large_jump<T>::type, std::integral_constant<int, 3> >));
}

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, HHoogland, International, Italian, Killer, Pool, Russian, Spanish
> Variants4;

BOOST_AUTO_TEST_CASE_TEMPLATE(LargeJump4, T, Variants4)
{
        BOOST_MPL_ASSERT((std::is_same< typename large_jump<T>::type, std::integral_constant<int, 4> >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
