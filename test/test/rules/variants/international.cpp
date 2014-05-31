#include <type_traits>                                  // is_same
#include <boost/mpl/assert.hpp>                         // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                         // vector
#include <boost/test/unit_test.hpp>                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/variants/akw_damme.hpp>            // AKWDamme
#include <dctl/rules/variants/h_hoogland.hpp>           // HHoogland
#include <dctl/rules/variants/international.hpp>        // International
#include <dctl/rules/variants/killer.hpp>               // Killer
#include <dctl/rules/traits.hpp>
#include <dctl/rules/types.hpp>

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RulesVariantsInternational)

// rules variants derived from international draughts
using VariantsDerivedFromInternational = boost::mpl::vector<
        AKWDamme, International, HHoogland, Killer
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(Traits, T, VariantsDerivedFromInternational)
{
        BOOST_MPL_ASSERT(( is_long_ranged_king_t<T> ));
        BOOST_MPL_ASSERT((std::is_same< typename directions::pawn_jump<T>::type, directions::diag     >));
        BOOST_MPL_ASSERT((std::is_same< typename      precedence::jump<T>::type, precedence::quantity >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
