//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/rules/type_traits.hpp>      // is_backward_pawn_jump, is_superior_rank_jump, is_orthogonal_jumps, is_passing_capture
#include <dctl/core/rules.hpp>                  // checkers, czech, frisian, international, italian, pool, russian, spanish, thai
#include <boost/mpl/vector.hpp>                 // vector
#include <boost/test/unit_test.hpp>             // BOOST_AUTO_TEST_SUITE, BOOST_AUTO_TEST_SUITE_END, BOOST_AUTO_TEST_CASE_TEMPLATE

using namespace dctl::core;

BOOST_AUTO_TEST_SUITE(IsBackwardPawnJump)

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

BOOST_AUTO_TEST_SUITE(IsOrthogonalJumps)

        using VariantsFalse = boost::mpl::vector
        <
                checkers, czech, international, italian, pool, russian, spanish, thai
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsFalse, T, VariantsFalse)
        {
                static_assert(!is_orthogonal_jumps_v<T>);
        }

        using VariantsTrue = boost::mpl::vector
        <
                frisian
        >;

        BOOST_AUTO_TEST_CASE_TEMPLATE(IsTrue, T, VariantsTrue)
        {
                static_assert(is_orthogonal_jumps_v<T>);
        }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(IsPawnsJumpKings)

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
