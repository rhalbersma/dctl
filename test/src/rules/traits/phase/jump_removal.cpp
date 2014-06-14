#include <dctl/rules/traits/is_en_passant_jump_removal.hpp>     // jump_removal
#include <dctl/rules/variants.hpp>
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsPhaseJumpRemoval)

using VariantsEnPassant = boost::mpl::vector<
        Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpRemovalPhaseIsEnPassant, Rules, VariantsEnPassant)
{
        static_assert(is_en_passant_jump_removal_v<Rules>, "");
}

using VariantsApresFini = boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpRemovalPhaseIsApresFini, Rules, VariantsApresFini)
{
        static_assert(!is_en_passant_jump_removal_v<Rules>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
