#include <dctl/rules/traits/precedence/is_absolute_king.hpp>    // is_absolute_king
#include <dctl/rules/variants.hpp>
#include <boost/mpl/assert.hpp>                                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsPrecedenceIsAbsoluteKing)

using VariantsFalseType = boost::mpl::vector<
        AKWDamme, Checkers, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsAbsoluteKingJumpPrecedenceIsFalseType, Rules, VariantsFalseType)
{
        BOOST_MPL_ASSERT_NOT((is_absolute_king_jump_precedence_t<Rules>));
}

using VariantsTrueType = boost::mpl::vector<
        Czech
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsAbsoluteKingJumpPrecedenceIsTrueType, Rules, VariantsTrueType)
{
        BOOST_MPL_ASSERT((is_absolute_king_jump_precedence_t<Rules>));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
