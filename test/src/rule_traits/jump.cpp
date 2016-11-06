#include <dctl/rule_traits.hpp>                 // is_backward_pawn_jump, is_superior_rank_jump, is_orthogonal_jump, is_passing_capture
#include <dctl/rules.hpp>                       // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(IsBackwardpawn_jump)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, italian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_backward_pawn_jump_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                frisian, international, pool, russian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_backward_pawn_jump_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsOrthogonalJump)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, international, italian, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_orthogonal_jump_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                frisian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_orthogonal_jump_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Ispawn_jumpKing)

        using VariantsTrue = boost::mpl::vector
        <
                checkers, czech, frisian, international, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsTrue)
        {
                static_assert(!is_superior_rank_jump_v<T>);
        }

        using VariantsFalse = boost::mpl::vector
        <
                italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsFalse)
        {
                static_assert(is_superior_rank_jump_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(JumpRemovalCategory)

        using VariantsStoppedJumpRemovalTag = boost::mpl::vector
        <
                checkers, czech, frisian, international, italian, pool, russian, spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsStoppedJumpRemovalTag, T, VariantsStoppedJumpRemovalTag)
        {
                static_assert(!is_passing_capture_v<T>);
        }

        using VariantsPassingJumpRemovalTag = boost::mpl::vector
        <
                thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsPassingJumpRemovalTag, T, VariantsPassingJumpRemovalTag)
        {
                static_assert(is_passing_capture_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
