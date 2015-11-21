#include <dctl/rule_traits.hpp>                 // king_range_category, long_ranged_tag, short_ranged_tag, is_land_behind_piece, is_halt_behind_king
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Killer, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <experimental/type_traits>             // is_same

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(KingRangeCategory)

        using VariantsShortRangedTag = boost::mpl::vector
        <
                Checkers, Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsSameAsShortRangedTag, T, VariantsShortRangedTag)
        {
                static_assert(std::experimental::is_same_v<king_range_category_t<T>, short_ranged_tag>);
        }

        using VariantsLongRangedTag = boost::mpl::vector
        <
                Czech, Frisian, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsSameAsLongRangedTag, T, VariantsLongRangedTag)
        {
                static_assert(std::experimental::is_same_v<king_range_category_t<T>, long_ranged_tag>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsLandBehindPiece)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_land_behind_piece_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_land_behind_piece_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsHaltBehindKing)

        using Variants = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalseT, T, Variants)
        {
                static_assert(!is_halt_behind_king_v<T>);
        }

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrueKillerT, T, Variants)
        {
                static_assert(is_halt_behind_king_v<Killer<T>>);
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
