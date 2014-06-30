#include <dctl/rules/traits/is_orthogonal_jump.hpp>     // is_orthogonal_jump_v
#include <dctl/rules/variants.hpp>
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsIsOrthogonalJump)

using VariantsFalse = boost::mpl::vector<
        AKWDamme, Checkers, Czech, HHoogland, International, Italian, Killer, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsIsOrthogonalJumpV, T, VariantsFalse)
{
        static_assert(!is_orthogonal_jump_v<T>, "");
}

using VariantsTrue = boost::mpl::vector<
        Frisian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsOrthogonalJumpV, T, VariantsTrue)
{
        static_assert(is_orthogonal_jump_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
