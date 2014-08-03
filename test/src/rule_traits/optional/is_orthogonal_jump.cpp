#include <dctl/rule_traits/optional/is_orthogonal_jump.hpp>     // is_orthogonal_jump
#include <dctl/rules.hpp>                                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsOrthogonalJump)

using VariantsFalse = boost::mpl::vector
<
        Checkers, Czech, International, Italian, Pool, Russian, Spanish, Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsIsOrthogonalJumpV, T, VariantsFalse)
{
        static_assert(!is_orthogonal_jump_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        Frisian
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsOrthogonalJumpV, T, VariantsTrue)
{
        static_assert(is_orthogonal_jump_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
