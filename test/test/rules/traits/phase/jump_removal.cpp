#include <dctl/rules/traits/phase/jump_removal.hpp>     // jump_removal
#include <dctl/rules/types/phase.hpp>                   // apres_fini, en_passant
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
        BOOST_MPL_ASSERT((std::is_same<jump_removal_phase_t<Rules>, en_passant>));
}

using VariantsApresFini = boost::mpl::vector<
        AKWDamme, Checkers, Czech, Frisian, HHoogland, International, Italian, Killer, Pool, Russian, Spanish
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(JumpRemovalPhaseIsApresFini, Rules, VariantsApresFini)
{
        BOOST_MPL_ASSERT((std::is_same<jump_removal_phase_t<Rules>, apres_fini>));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
