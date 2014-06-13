#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/precedence/jump.hpp>        // jump
#include <dctl/rules/types/precedence.hpp>              // none, quantity, quality
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace precedence {

BOOST_AUTO_TEST_SUITE(RulesTraitsPrecedenceJump)

typedef boost::mpl::vector<
        Checkers, Czech, Pool, Russian, Thai
> VariantsNone;

BOOST_AUTO_TEST_CASE_TEMPLATE(None, T, VariantsNone)
{
        BOOST_MPL_ASSERT((std::is_same< typename jump<T>::type, none >));
}

typedef boost::mpl::vector<
        AKWDamme, HHoogland, International, Killer
> VariantsQuantity;

BOOST_AUTO_TEST_CASE_TEMPLATE(Quantity, T, VariantsQuantity)
{
        BOOST_MPL_ASSERT((std::is_same< typename jump<T>::type, quantity >));
}

typedef boost::mpl::vector<
        Frisian, Italian, Spanish
> VariantsQuality;

BOOST_AUTO_TEST_CASE_TEMPLATE(Quality, T, VariantsQuality)
{
        BOOST_MPL_ASSERT((std::is_same< typename jump<T>::type, quality >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
