#include <dctl/rule_traits/optional/is_directly_land_after_piece.hpp>   // is_directly_land_after_piece
#include <dctl/rules.hpp>                                               // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                                         // vector
#include <boost/test/test_case_template.hpp>                            // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>                                     // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(RuleTraitsIsDirectlyLandAfterPiece)

using VariantsFalse = boost::mpl::vector
<
        checkers::Rules, czech::Rules, frisian::Rules, international::Rules, italian::Rules, pool::Rules, russian::Rules, spanish::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(NotIsDirectlyLandAfterPieceV, T, VariantsFalse)
{
        static_assert(!is_directly_land_after_piece_v<T>, "");
}

using VariantsTrue = boost::mpl::vector
<
        thai::Rules
>;

BOOST_AUTO_TEST_CASE_TEMPLATE(IsDirectlyLandAfterPieceV, T, VariantsTrue)
{
        static_assert(is_directly_land_after_piece_v<T>, "");
}

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
