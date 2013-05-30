#include <type_traits>                          // is_same
#include <boost/mpl/assert.hpp>                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_CASE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/types.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(TestInternational)

// rules variants derived from international draughts
typedef boost::mpl::vector<
        rules::International, rules::Damme, rules::Hoogland, rules::Killer
> DerivedFromInternational;

BOOST_AUTO_TEST_CASE_TEMPLATE(Traits, T, DerivedFromInternational)
{
        BOOST_MPL_ASSERT((std::is_same< typename           range::scan<T>::type, range::distance_N    >));
        BOOST_MPL_ASSERT((std::is_same< typename directions::pawn_jump<T>::type, directions::diag     >));
        BOOST_MPL_ASSERT((std::is_same< typename      precedence::jump<T>::type, precedence::quantity >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
