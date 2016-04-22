#include <dctl/rule_traits.hpp>                 // king_range_category, long_ranged_tag, short_ranged_tag, is_land_behind_piece, is_halt_behind_king
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, killer, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(KingRangeCategory)

        using VariantsShortRangedTag = boost::mpl::vector
        <
                checkers, italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsSameAsShortRangedTag, T, VariantsShortRangedTag)
        {
                static_assert(!is_long_ranged_king_v<T>);
        }

        using VariantsLongRangedTag = boost::mpl::vector
        <
                czech, frisian, international, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsSameAsLongRangedTag, T, VariantsLongRangedTag)
        {
                static_assert(is_long_ranged_king_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsLandBehindPiece)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, frisian, international, italian, pool, russian, spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_land_behind_piece_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_land_behind_piece_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsHaltBehindKing)

        using Variants = boost::mpl::vector
        <
                checkers, czech, frisian, international, italian, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalseT, T, Variants)
        {
                static_assert(!is_halt_behind_king_v<T>);
        }

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTruekillerT, T, Variants)
        {
                static_assert(is_halt_behind_king_v<killer<T>>);
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
