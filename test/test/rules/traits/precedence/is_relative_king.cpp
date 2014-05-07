#include <dctl/rules/traits/precedence/is_relative_king.hpp>    // is_relative_king
#include <dctl/rules/variants.hpp>
#include <boost/mpl/assert.hpp>                                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                                          // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsPrecedenceIsRelativeKing)

using VariantsFalseType = boost::mpl::vector<
        AKWDamme, Checkers, Czech, HHoogland, International, Killer, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRelativeKingJumpPrecedenceIsFalseType, Rules, VariantsFalseType)
{
        BOOST_MPL_ASSERT_NOT((is_relative_king_jump_precedence_t<Rules>));
}

using VariantsTrueType = boost::mpl::vector<
        Frisian, Italian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRelativeKingJumpPrecedenceIsTrueType, Rules, VariantsTrueType)
{
        BOOST_MPL_ASSERT((is_relative_king_jump_precedence_t<Rules>));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
