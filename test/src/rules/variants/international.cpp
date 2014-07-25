#include <dctl/rules/variants/international.hpp>        // International
#include <dctl/rules/variants/killer.hpp>               // Killer
#include <dctl/rules/traits.hpp>
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsInternational)

// rules variants derived from international draughts
using VariantsDerivedFromInternational = boost::mpl::vector<
        International, Killer
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Traits, T, VariantsDerivedFromInternational)
{
        static_assert(is_long_ranged_king_v<T>, "");
        static_assert(is_backward_pawn_jump_v<T>, "");
        static_assert(is_jump_precedence_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
