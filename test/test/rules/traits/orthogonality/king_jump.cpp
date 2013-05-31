#include <type_traits>                                          // is_same
#include <boost/mpl/assert.hpp>                                 // BOOST_MPL_ASSERT
#include <boost/mpl/vector.hpp>                                 // vector
#include <boost/test/unit_test.hpp>                             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>                    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <dctl/rules/traits/orthogonality/king_jump.hpp>        // king_jump
#include <dctl/rules/types/orthogonality.hpp>                   // none, relative, absolute
#include <dctl/rules/variants.hpp>

namespace dctl {
namespace rules {
namespace orthogonality {

BOOST_AUTO_TEST_SUITE(RulesTraitsOrthogonalityKingJump)

typedef boost::mpl::vector<
        AKWDamme, Checkers, Czech, International, Italian, Killer, Pool, Russian, Spanish, Thai
> VariantsNone;

BOOST_AUTO_TEST_CASE_TEMPLATE(None, T, VariantsNone)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_jump<T>::type, none >));
}

typedef boost::mpl::vector<
        HHoogland
> VariantsRelative;

BOOST_AUTO_TEST_CASE_TEMPLATE(Relative, T, VariantsRelative)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_jump<T>::type, relative >));
}

typedef boost::mpl::vector<
        Frisian
> VariantsAbsolute;

BOOST_AUTO_TEST_CASE_TEMPLATE(Absolute, T, VariantsAbsolute)
{
        BOOST_MPL_ASSERT((std::is_same< typename king_jump<T>::type, absolute >));
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace orthogonality
}       // namespace rules
}       // namespace dctl
