#include <dctl/rule_traits.hpp>                 // is_en_passant_jump_removal_v
#include <dctl/rules.hpp>
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsIsEnPassantJumpRemoval)

using VariantsFalse = boost::mpl::vector<
        Checkers, Czech, Frisian, International, Italian, Killer, Pool, Russian, Spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsEnPassantJumpRemovalV, T, VariantsFalse)
{
        static_assert(!is_en_passant_jump_removal_v<T>, "");
}

using VariantsTrue = boost::mpl::vector<
        Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsEnPassantJumpRemovalV, T, VariantsTrue)
{
        static_assert(is_en_passant_jump_removal_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
