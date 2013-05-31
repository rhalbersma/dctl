#include <type_traits>                                          // is_same
#include <boost/mpl/assert.hpp>                                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/precedence/is_relative_king.hpp>    // is_relative_king
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace precedence {

BOOST_AUTO_TEST_SUITE(RulesTraitsPrecedenceIsRelativeKing)

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, HHoogland, International, Killer, Pool, Russian, Spanish, Thai
> VariantsFalseType;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRelativeKingFalseType, T, VariantsFalseType)
{
        BOOST_MPL_ASSERT_NOT(( is_relative_king<T> ));
}

typedef boost::mpl::vector<
        Frisian, Italian
> VariantsTrueType;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRelativeKingTrueType, T, VariantsTrueType)
{
        BOOST_MPL_ASSERT(( is_relative_king<T> ));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace precedence
}       // namespace rules
}       // namespace dctl
