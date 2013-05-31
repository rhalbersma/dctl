#include <type_traits>                                          // is_same
#include <boost/mpl/assert.hpp>                                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/precedence/is_absolute_king.hpp>    // is_absolute_king
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace precedence {

BOOST_AUTO_TEST_SUITE(RulesTraitsPrecedenceIsAbsoluteKing)

typedef boost::mpl::vector<
        AKWDamme, Checkers, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish, Thai
> VariantsFalseType;

BOOST_AUTO_TEST_CASE_TEMPLATE(FalseType, T, VariantsFalseType)
{
        BOOST_MPL_ASSERT_NOT(( is_absolute_king<T> ));
}

typedef boost::mpl::vector<
        Czech
> VariantsTrueType;

BOOST_AUTO_TEST_CASE_TEMPLATE(TrueType, T, VariantsTrueType)
{
        BOOST_MPL_ASSERT(( is_absolute_king<T> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
