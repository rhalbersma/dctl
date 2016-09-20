#include <dctl/rule_traits.hpp>                 // is_backward_pawn_jump, is_superior_rank_jump, is_orthogonal_jump, is_passing_capture
#include <dctl/rules.hpp>                       // Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish, Thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/test_case_template.hpp>    // BOOST_AUTO_TEST_CASE_TEMPLATE
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END

namespace dctl {
namespace rules {

BOOST_AUTO_TEST_SUITE(IsBackwardpawn_jump)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, Italian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_backward_pawn_jump_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Frisian, International, Pool, Russian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_backward_pawn_jump_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsOrthogonalJump)

        using VariantsFalse = boost::mpl::vector
        <
                Checkers, Czech, International, Italian, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_orthogonal_jump_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                Frisian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_orthogonal_jump_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Ispawn_jumpKing)

        using VariantsTrue = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Pool, Russian, Spanish, Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsTrue)
        {
                static_assert(!is_superior_rank_jump_v<T>);
        }

        using VariantsFalse = boost::mpl::vector
        <
                Italian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsFalse)
        {
                static_assert(is_superior_rank_jump_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(JumpRemovalCategory)

        using VariantsStoppedJumpRemovalTag = boost::mpl::vector
        <
                Checkers, Czech, Frisian, International, Italian, Pool, Russian, Spanish
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsStoppedJumpRemovalTag, T, VariantsStoppedJumpRemovalTag)
        {
                static_assert(!is_passing_capture_v<T>);
        }

        using VariantsPassingJumpRemovalTag = boost::mpl::vector
        <
                Thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsPassingJumpRemovalTag, T, VariantsPassingJumpRemovalTag)
        {
                static_assert(is_passing_capture_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

}       // namespace rules
}       // namespace dctl
