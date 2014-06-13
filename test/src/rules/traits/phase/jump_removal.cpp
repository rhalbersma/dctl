#include <dctl/rules/traits/is_en_passant_jump_removal.hpp>     // jump_removal
#include <dctl/rules/variants.hpp>
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <type_traits>                                  // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesTraitsPhaseJumpRemoval)

using VariantsEnPassant = boost::mpl::vector<
        Thai
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpRemovalPhaseIsEnPassant, Rules, VariantsEnPassant)
{
        BOOST_MPL_ASSERT((std::is_same<is_en_passant_jump_removal_t<Rules>, std::true_type>));
}

using VariantsApresFini = boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpRemovalPhaseIsApresFini, Rules, VariantsApresFini)
{
        BOOST_MPL_ASSERT((std::is_same<is_en_passant_jump_removal_t<Rules>, std::false_type>));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
