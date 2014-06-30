#include <dctl/rules/traits/is_relative_king_jump_precedence.hpp>       // is_relative_king_jump_precedence_v
#include <dctl/rules/variants.hpp>
#include <boost/mpl/vector.hpp>                                         // vector
#include <boost/test/test_case_template.hpp>                            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsIsRelativeKingPrecedence)

using VariantsFalse = boost::mpl::vector<
        AKWDamme, Checkers, Czech, HHoogland, International, Killer, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsRelativeKingJumpPrecedenceV, T, VariantsFalse)
{
        static_assert(!is_relative_king_jump_precedence_v<T>, "");
}

using VariantsTrue = boost::mpl::vector<
        Frisian, Italian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsRelativeKingJumpPrecedenceV, T, VariantsTrue)
{
        static_assert(is_relative_king_jump_precedence_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
